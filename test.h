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
    Config config = Config::getConfig();
    config.read("conf.d");
    FBPInit().run();
    PlayerManager& manager = PlayerManager::getManager();
    foreach(QString playername, config.getPlayers())
    {
        Player& p = manager.getPlayer(playername);
        workersOrdered
        workersSalary
        engineersOrdered
        engineersSalary
        productsOrdered
        qualityCost
        price
        salesBudget
        saleAgentAdded
        saleAgentRemoved
        marketsOrderedReport
        hireConsultant
        moneyPaidForLoan

    }
}

#endif // TEST_H
