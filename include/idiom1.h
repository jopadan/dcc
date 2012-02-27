#pragma once
#include "idiom.h"
struct Idiom1 : public Idiom
{
protected:
    std::vector<iICODE> m_icodes;
    int m_min_off;
    Int checkStkVars (iICODE pIcode);
public:
    Idiom1(Function *f) : Idiom(f)
    {
    }
    uint8_t minimum_match_length() {return 1;}
    bool match(iICODE picode);
    int action();
};