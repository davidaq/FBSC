#include <QtCore>
#include <QMap>
#include "fbploan.h"
#include "fbpinit.h"
#include "fbpproduce.h"
#include "fbpsales.h"
#include "fbphire.h"
#include "fbpbonus.h"
#include "fbpend.h"
#include "fbpreport.h"
#include "fbpstartround.h"
#include "FBProcess.h"
#include "config.h"
#include "playermanager.h"
#include <QApplication>

//#define TEST
#ifdef TEST

#include "test.h"
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
    QApplication app(argc, argv);
    QList<NamedProcess> processes;
#define REGISTER(NAME, OBJ) processes.append(qMakePair(QString(NAME), (FBProcess*) (new OBJ)))

    // Register processes here
    REGISTER("init", FBPInit);
    REGISTER("start", FBPStartRound);
    REGISTER("loan", FBPLoan);
    REGISTER("hire", FBPHire);
    REGISTER("produce", FBPProduce);
    REGISTER("sales", FBPSales);
    REGISTER("end", FBPEnd);
    REGISTER("bonus", FBPBonus);
    REGISTER("report", FBPReport);

    // End of register block

    QString confFile = "conf.d";

    if(argv[1][0] == ':')
    {
        confFile = argv[1];
        confFile.remove(0, 1);
    }
    qDebug() << confFile;
    do {
        QFile file(confFile);
        if(file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            if(!file.atEnd()) {
                Config::getConfig().read(stream);
            }
            if(!file.atEnd()) {
                PlayerManager::getManager().read(stream);
            }
            file.close();
        }
    } while(false);

    int run(QString procName, const QList<NamedProcess> & processes);
    for(int i = 1; i < argc; i++) {
        if(i > 1 || argv[i][0] != ':')
        {
            int r = run(argv[i], processes);
            if(r != 0) {
                qDebug()<<"error";
                return r;
            }
        }
    }

    QFile file(confFile);
    if(file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        Config::getConfig().write(stream);
        PlayerManager::getManager().write(stream);
        file.close();
    }
    return 0;
}

int run(QString procName, const QList<NamedProcess> & processes) {
    FBProcess* proc = 0;
    foreach(NamedProcess np, processes) {
        if(np.first == procName) {
            proc = np.second;
        }
    }
    if(proc == 0) {
        qDebug()<<"Process [" + procName + "] doesn't exist";
        return 1;
    } else {
        int ret = proc->run();
        qDebug()<<procName;
        PlayerManager::getManager().flush(procName + ".stat");
        return ret;
    }
}

#endif
