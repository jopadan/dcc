#include <cassert>
#include <string>
#include <boost/range/rbegin.hpp>
#include <boost/range/rend.hpp>
#include <boost/range/adaptors.hpp>
#include "BasicBlock.h"
#include "Procedure.h"
#include "dcc.h"
using namespace std;
using namespace boost;

BB *BB::Create(void */*ctx*/, const string &/*s*/, Function *parent, BB */*insertBefore*/)
{
    BB *pnewBB = new BB;
    pnewBB->Parent = parent;
    return pnewBB;
}
/**
 *  @arg start - basic block starts here, might be parent->Icode.end()
 *  @arg fin - last of basic block's instructions
*/
BB *BB::Create(iICODE start, iICODE fin, uint8_t _nodeType, int numOutEdges, Function *parent)
{
    BB* pnewBB;
    pnewBB = new BB;
    pnewBB->nodeType = _nodeType;	/* Initialise */
    pnewBB->immedDom = NO_DOM;
    pnewBB->loopHead = pnewBB->caseHead = pnewBB->caseTail =
    pnewBB->latchNode= pnewBB->loopFollow = NO_NODE;
    pnewBB->instructions = make_iterator_range(start,fin);
    if(start==parent->Icode.end())
    {
        pnewBB->instructions = make_iterator_range(parent->Icode.end(),parent->Icode.end());
    }
    else
    {
        pnewBB->instructions.advance_end(1); // 1 after fin, to create range where fin is inclusive
    }
    if (numOutEdges)
        pnewBB->edges.resize(numOutEdges);

    /* Mark the basic block to which the icodes belong to, but only for
         * real code basic blocks (ie. not interval bbs) */
    if(parent)
    {
        if (start != parent->Icode.end())
            parent->Icode.SetInBB(pnewBB->instructions, pnewBB);
        parent->heldBBs.push_back(pnewBB);
        parent->m_cfg.push_back(pnewBB);
        pnewBB->Parent = parent;
    }
    if ( start != parent->Icode.end() )		/* Only for code BB's */
        stats.numBBbef++;
    return pnewBB;
}
BB *BB::Create(int start, int ip, uint8_t _nodeType, int numOutEdges, Function *parent)
{
    iICODE st(parent->Icode.begin());
    iICODE fin(parent->Icode.begin());
    if(start==-1)
    {
        st  = parent->Icode.end();
        fin = parent->Icode.end();
    }
    else
    {
        advance(st,start);
        advance(fin,ip);
    }
    return Create(st,fin,_nodeType,numOutEdges,parent);
}

static const char *const s_nodeType[] = {"branch", "if", "case", "fall", "return", "call",
                                 "loop", "repeat", "interval", "cycleHead",
                                 "caseHead", "terminate",
                                 "nowhere" };

static const char *const s_loopType[] = {"noLoop", "while", "repeat", "loop", "for"};


void BB::display()
{
    printf("\nnode type = %s, ", s_nodeType[nodeType]);
    printf("start = %d, length = %zd, #out edges = %zd\n", begin()->loc_ip, size(), edges.size());

    for (size_t i = 0; i < edges.size(); i++)
    {
        if(edges[i].BBptr==0)
            printf(" outEdge[%2zd] = Unlinked out edge to %d\n",i, edges[i].ip);
        else
            printf(" outEdge[%2zd] = %d\n",i, edges[i].BBptr->begin()->loc_ip);
    }
}
/*****************************************************************************
 * displayDfs - Displays the CFG using a depth first traversal
 ****************************************************************************/
void BB::displayDfs()
{
    int i;
    assert(this);
    traversed = DFS_DISP;

    printf("node type = %s, ", s_nodeType[nodeType]);
    printf("start = %d, length = %zd, #in-edges = %zd, #out-edges = %zd\n",
           begin()->loc_ip, size(), inEdges.size(), edges.size());
    printf("dfsFirst = %d, dfsLast = %d, immed dom = %d\n",
           dfsFirstNum, dfsLastNum,
           immedDom == MAX ? -1 : immedDom);
    printf("loopType = %s, loopHead = %d, latchNode = %d, follow = %d\n",
           s_loopType[loopType],
           loopHead == MAX ? -1 : loopHead,
           latchNode == MAX ? -1 : latchNode,
           loopFollow == MAX ? -1 : loopFollow);
    printf ("ifFollow = %d, caseHead = %d, caseTail = %d\n",
            ifFollow == MAX ? -1 : ifFollow,
            caseHead == MAX ? -1 : caseHead,
            caseTail == MAX ? -1 : caseTail);

    if (nodeType == INTERVAL_NODE)
        printf("corresponding interval = %d\n", correspInt->numInt);
    else
    {
        int edge_idx=0;
        for(BB *node : inEdges)
        {
            printf ("  inEdge[%d] = %d\n", edge_idx, node->begin()->loc_ip);
            edge_idx++;
        }
    }
    /* Display out edges information */
    i=0;
    for(TYPEADR_TYPE &edg : edges)
    {
        if (nodeType == INTERVAL_NODE)
            printf(" outEdge[%d] = %d\n", i, edg.BBptr->correspInt->numInt);
        else
            printf(" outEdge[%d] = %d\n", i, edg.BBptr->begin()->loc_ip);
        ++i;
    }
    printf("----\n");

    /* Recursive call on successors of current node */
    for(TYPEADR_TYPE &pb : edges)
    {
        if (pb.BBptr->traversed != DFS_DISP)
            pb.BBptr->displayDfs();
    }
}
/* Recursive procedure that writes the code for the given procedure, pointed
 * to by pBB.
 * Parameters:	pBB:	pointer to the cfg.
 *				Icode:	pointer to the Icode array for the cfg graph of the
 *						current procedure.
 *				indLevel: indentation level - used for formatting.
 *				numLoc: last # assigned to local variables 				*/
ICODE* BB::writeLoopHeader(int &indLevel, Function* pProc, int *numLoc, BB *&latch, boolT &repCond)
{
    latch = pProc->m_dfsLast[this->latchNode];
    std::ostringstream ostr;
    ICODE* picode;
    switch (loopType)
    {
        case WHILE_TYPE:
            picode = &this->back();

            /* Check for error in while condition */
            if (picode->hl()->opcode != HLI_JCOND)
                reportError (WHILE_FAIL);

            /* Check if condition is more than 1 HL instruction */
            if (numHlIcodes > 1)
            {
                /* Write the code for this basic block */
                writeBB(ostr,indLevel, pProc, numLoc);
                repCond = true;
            }

            /* Condition needs to be inverted if the loop body is along
             * the THEN path of the header node */
            if (edges[ELSE].BBptr->dfsLastNum == loopFollow)
            {
                picode->hl()->replaceExpr(picode->hl()->expr()->inverse());
            }
            {
                string e=walkCondExpr (picode->hl()->expr(), pProc, numLoc);
                ostr << "\n"<<indentStr(indLevel)<<"while ("<<e<<") {\n";
            }
            picode->invalidate();
            break;

        case REPEAT_TYPE:
            ostr << "\n"<<indentStr(indLevel)<<"do {\n";
            picode = &latch->back();
            picode->invalidate();
            break;

        case ENDLESS_TYPE:
            ostr << "\n"<<indentStr(indLevel)<<"for (;;) {\n";
    }
    cCode.appendCode(ostr.str());
    stats.numHLIcode += 1;
    indLevel++;
    return picode;
}
bool BB::isEndOfPath(int latch_node_idx) const
{
    return nodeType == RETURN_NODE || nodeType == TERMINATE_NODE ||
           nodeType == NOWHERE_NODE || (dfsLastNum == latch_node_idx);
}
void BB::writeCode (int indLevel, Function * pProc , int *numLoc,int _latchNode, int _ifFollow)
{
    int follow;						/* ifFollow                 	*/
    BB * succ, *latch;					/* Successor and latching node 	*/
    ICODE * picode;					/* Pointer to HLI_JCOND instruction	*/
    char *l;                                            /* Pointer to HLI_JCOND expression	*/
    boolT emptyThen,					/* THEN clause is empty			*/
            repCond;					/* Repeat condition for while() */

    /* Check if this basic block should be analysed */
    if ((_ifFollow != UN_INIT) && (this == pProc->m_dfsLast[_ifFollow]))
        return;

    if (wasTraversedAtLevel(DFS_ALPHA))
        return;
    traversed = DFS_ALPHA;

    /* Check for start of loop */
    repCond = false;
    latch = NULL;
    if (loopType)
    {
       picode=writeLoopHeader(indLevel, pProc, numLoc, latch, repCond);
    }

    /* Write the code for this basic block */
    if (repCond == false)
    {
        std::ostringstream ostr;
        writeBB(ostr,indLevel, pProc, numLoc);
        cCode.appendCode(ostr.str());
    }

    /* Check for end of path */
    if (isEndOfPath(_latchNode))
        return;

    /* Check type of loop/node and process code */
    if (    loopType)	/* there is a loop */
    {
        assert(latch);
        if (this != latch)		/* loop is over several bbs */
        {
            if (loopType == WHILE_TYPE)
            {
                succ = edges[THEN].BBptr;
                if (succ->dfsLastNum == loopFollow)
                    succ = edges[ELSE].BBptr;
            }
            else
                succ = edges[0].BBptr;
            if (succ->traversed != DFS_ALPHA)
                succ->writeCode (indLevel, pProc, numLoc, latch->dfsLastNum,_ifFollow);
            else	/* has been traversed so we need a goto */
                succ->front().ll()->emitGotoLabel (indLevel);
        }

        /* Loop epilogue: generate the loop trailer */
        indLevel--;
        if (loopType == WHILE_TYPE)
        {
            std::ostringstream ostr;
            /* Check if there is need to repeat other statements involved
                         * in while condition, then, emit the loop trailer */
            if (repCond)
            {
                writeBB(ostr,indLevel+1, pProc, numLoc);
            }
            ostr <<indentStr(indLevel)<< "}	/* end of while */\n";
            cCode.appendCode(ostr.str());
        }
        else if (loopType == ENDLESS_TYPE)
            cCode.appendCode( "%s}	/* end of loop */\n",indentStr(indLevel));
        else if (loopType == REPEAT_TYPE)
        {
            if (picode->hl()->opcode != HLI_JCOND)
                reportError (REPEAT_FAIL);
            {
                string e=walkCondExpr (picode->hl()->expr(), pProc, numLoc);
                cCode.appendCode( "%s} while (%s);\n", indentStr(indLevel),e.c_str());
            }
        }

        /* Recurse on the loop follow */
        if (loopFollow != MAX)
        {
            succ = pProc->m_dfsLast[loopFollow];
            if (succ->traversed != DFS_ALPHA)
                succ->writeCode (indLevel, pProc, numLoc, _latchNode, _ifFollow);
            else		/* has been traversed so we need a goto */
                succ->front().ll()->emitGotoLabel (indLevel);
        }
    }

    else		/* no loop, process nodeType of the graph */
    {
        if (nodeType == TWO_BRANCH)		/* if-then[-else] */
        {
            stats.numHLIcode++;
            indLevel++;
            emptyThen = false;

            if (ifFollow != MAX)		/* there is a follow */
            {
                /* process the THEN part */
                follow = ifFollow;
                succ = edges[THEN].BBptr;
                if (succ->traversed != DFS_ALPHA)	/* not visited */
                {
                    if (succ->dfsLastNum != follow)	/* THEN part */
                    {
                        l = writeJcond ( *back().hl(), pProc, numLoc);
                        cCode.appendCode( "\n%s%s", indentStr(indLevel-1), l);
                        succ->writeCode (indLevel, pProc, numLoc, _latchNode,follow);
                    }
                    else		/* empty THEN part => negate ELSE part */
                    {
                        l = writeJcondInv ( *back().hl(), pProc, numLoc);
                        cCode.appendCode( "\n%s%s", indentStr(indLevel-1), l);
                        edges[ELSE].BBptr->writeCode (indLevel, pProc, numLoc, _latchNode, follow);
                        emptyThen = true;
                    }
                }
                else	/* already visited => emit label */
                    succ->front().ll()->emitGotoLabel(indLevel);

                /* process the ELSE part */
                succ = edges[ELSE].BBptr;
                if (succ->traversed != DFS_ALPHA)		/* not visited */
                {
                    if (succ->dfsLastNum != follow)		/* ELSE part */
                    {
                        cCode.appendCode( "%s}\n%selse {\n",
                                          indentStr(indLevel-1), indentStr(indLevel - 1));
                        succ->writeCode (indLevel, pProc, numLoc, _latchNode, follow);
                    }
                    /* else (empty ELSE part) */
                }
                else if (! emptyThen) 	/* already visited => emit label */
                {
                    cCode.appendCode( "%s}\n%selse {\n",
                                      indentStr(indLevel-1), indentStr(indLevel - 1));
                    succ->front().ll()->emitGotoLabel (indLevel);
                }
                cCode.appendCode( "%s}\n", indentStr(--indLevel));

                /* Continue with the follow */
                succ = pProc->m_dfsLast[follow];
                if (succ->traversed != DFS_ALPHA)
                    succ->writeCode (indLevel, pProc, numLoc, _latchNode,_ifFollow);
            }
            else		/* no follow => if..then..else */
            {
                l = writeJcond ( *back().hl(), pProc, numLoc);
                cCode.appendCode( "%s%s", indentStr(indLevel-1), l);
                edges[THEN].BBptr->writeCode (indLevel, pProc, numLoc, _latchNode, _ifFollow);
                cCode.appendCode( "%s}\n%selse {\n", indentStr(indLevel-1), indentStr(indLevel - 1));
                edges[ELSE].BBptr->writeCode (indLevel, pProc, numLoc, _latchNode, _ifFollow);
                cCode.appendCode( "%s}\n", indentStr(--indLevel));
            }
        }

        else 	/* fall, call, 1w */
        {
            succ = edges[0].BBptr;		/* fall-through edge */
            assert(succ->size()>0);
            if (succ->traversed != DFS_ALPHA)
            {
                succ->writeCode (indLevel, pProc,numLoc, _latchNode,_ifFollow);
            }
        }
    }
}
/* Writes the code for the current basic block.
 * Args: pBB: pointer to the current basic block.
 *		 Icode: pointer to the array of icodes for current procedure.
 *		 lev: indentation level - used for formatting.	*/
void BB::writeBB(std::ostream &ostr,int lev, Function * pProc, int *numLoc)
{
    /* Save the index into the code table in case there is a later goto
     * into this instruction (first instruction of the BB) */
    front().ll()->codeIdx = cCode.code.nextIdx();

    /* Generate code for each hlicode that is not a HLI_JCOND */

    for(ICODE &pHli : instructions)
    {
        if ((pHli.type == HIGH_LEVEL) && ( pHli.valid() )) //TODO: use filtering range here.
        {
            std::string line = pHli.hl()->write1HlIcode(pProc, numLoc);
            if (!line.empty())
            {
                ostr<<indentStr(lev)<<line;
                stats.numHLIcode++;
            }
            if (option.verbose)
                pHli.writeDU();
        }
    }
}

iICODE BB::begin()
{
    return instructions.begin();//range_start;
}

iICODE BB::end() const
{
    return instructions.end();//range_end
}
ICODE &BB::back()
{
    return instructions.back();//*rbegin();
}

size_t BB::size()
{

    return distance(instructions.begin(),instructions.end());
}

ICODE &BB::front()
{
    return instructions.front();//*begin();
}

riICODE BB::rbegin()
{
    return riICODE( instructions.end() );
}
riICODE BB::rend()
{
    return riICODE( instructions.begin() );
}
