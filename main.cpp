#include <QtCore>
#include <QMap>
#include "fbploan.h"
#include "FBProcess.h"

int run(QString procName) {
    typedef QPair<QString, FBProcess*> NamedProcess;
    QList<NamedProcess> processes;
#define REGISTER(NAME, OBJ) processes.append(qMakePair(QString(NAME), (FBProcess*) (new OBJ)))

    // Register processes here
    REGISTER("loan", FBPLoan);
    // End of register block

    FBProcess* proc = 0;
    foreach(NamedProcess np, processes) {
        if(np.first == procName) {
            proc = np.second;
        } else {
            delete np.second;
        }
    }
    if(proc == 0) {
        qDebug()<<"Process [" + procName + "] doesn't exist";
        return 1;
    } else {
        return proc->run();
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        qDebug()<<"Please specify a process to run";
        return 1;
    }
    run(argv[1]);
}
