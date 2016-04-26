#pragma once
#include <QtCore/QObject>
class Project;
class DccFrontend : public QObject
{
    Q_OBJECT
    void    LoadImage();
    void    parse(Project &proj);
public:
    explicit DccFrontend(QObject *parent = 0);
    bool FrontEnd();            /* frontend.c   */
    void initializeMachineState(Project & proj);

    void createEntryProc(Project &proj);

signals:

public slots:
};
