#ifndef TEST_H
#define TEST_H

#include "fbploan.h"
#include "fbpinit.h"
#include "fbpproduce.h"
#include "fbpsales.h"
#include "fbphire.h"
#include "FBProcess.h"
#include "config.h"
#include "playermanager.h"

void genDefaultConfig()
{
    Config::getConfig().genDefault();
}

void genDec()
{
    Config& config = Config::getConfig();
    config.read("conf.d");
    FBPInit().run();
    PlayerManager& manager = PlayerManager::getManager();
	do {
        Player& p = manager.getPlayer("p1");
        p.record["workersOrdered"] = QString::number(50);
        p.record["workersSalary"] = QString::number(100);
        p.record["engineersOrdered"] = QString::number(100);
        p.record["engineersSalary"] = QString::number(300);
        p.record["productsOrdered"] = QString::number(20000);
        p.record["qualityCost"] = QString::number(500);
        p.record["price"] = QString::number(10000);
        p.record["salesSupport"] = QString::number(2);
        p.record["saleAgentAdded"] = "1;;0";
        p.record["saleAgentRemoved"] = "";
        p.record["marketsOrderedReport"] = "1;;0";
        p.record["hireConsultant"] = QString::number(1);
        p.record["loan"] = QString::number(100);
	} while (false);
	do {
        Player& p = manager.getPlayer("p2");
        p.record["workersOrdered"] = QString::number(70);
        p.record["workersSalary"] = QString::number(200);
        p.record["engineersOrdered"] = QString::number(100);
        p.record["engineersSalary"] = QString::number(900);
        p.record["productsOrdered"] = QString::number(30000);
        p.record["qualityCost"] = QString::number(300);
        p.record["price"] = QString::number(12000);
        p.record["salesSupport"] = QString::number(999);
        p.record["saleAgentAdded"] = "";
        p.record["saleAgentRemoved"] = "";
        p.record["marketsOrderedReport"] = "0";
        p.record["hireConsultant"] = QString::number(0);
        p.record["loan"] = QString::number(0);
	} while (false);
    QFile file("conf.d");
    if(file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        config.write(stream);
        manager.write(stream);
        file.close();
    }
}

#endif // TEST_H
