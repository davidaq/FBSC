#include <QtCore>
#include <QMap>
#include "fbploan.h"
#include "fbpinit.h"
#include "fbpproduce.h"
#include "fbpsales.h"
#include "fbphire.h"
#include "FBProcess.h"
#include "config.h"
#include "playermanager.h"
#include "test.h"

#define TEST
#ifdef TEST

int main()
{
    genDefaultConfig();
    genDec();
    return 0;
}

#else

typedef QPair<QString, FBProcess*> NamedProcess;

int main(int argc, char *argv[])
{
    QList<NamedProcess> processes;
#define REGISTER(NAME, OBJ) processes.append(qMakePair(QString(NAME), (FBProcess*) (new OBJ)))


    // Register processes here
    REGISTER("init", FBPLoan);
    REGISTER("loan", FBPLoan);
    REGISTER("hire", FBPHire);
    REGISTER("produce", FBPProduce);
    REGISTER("sales", FBPSales);

    // End of register block
    Config::getConfig().read("conf.d");
    PlayerManager::getManager().read("conf.d");

    int run(QString procName, const QList<NamedProcess> & processes);
    for(int i = 1; i < argc; i++) {
        int r = run(argv[i], processes);
        if(r != 0)
            return r;
    }
    return 0;
}

int run(QString procName, const QList<NamedProcess> & processes) {
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

#endif
