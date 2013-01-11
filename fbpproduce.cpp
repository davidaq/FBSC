#include "fbpproduce.h"
#include "config.h"
#include "playermanager.h"

int FBPProduce::run()
{
    PlayerManager manager = PlayerManager::getManager();
    Config config = Config::getConfig();
    int workersPerTeam = config.get("Workers in a component").toInt();
    int engineersPerTeam = config.get("Engineers in a product").toInt();
    int comMatCost = config.get("Material cost per component").toInt();
    int comPerPrd = config.get("Components in a product").toInt();
    int prdMatCost = config.get("Material cost per product").toInt();
    int comStrCost = config.get("Component storage cost per quarter").toInt();
    int hours = config.get("Month in a quarter").toInt()
            * config.get("Days in a month").toInt()
            * config.get("Working hours in a month").toInt();
    foreach(QString name, config.getPlayers()) {
        Player player = manager.getPlayer(name);
        int workers = player.record["workersHired"].toInt();
        int engineers = player.record["engineersHired"].toInt();
        int wp = workers / workersPerTeam;
        int ep= engineers / engineersPerTeam;
        player.record["workersWorking"] = wp * workersPerTeam;
        player.record["workersIdle"] = workers - wp * workersPerTeam;
        player.record["engineersWorking"] = ep * engineersPerTeam;
        player.record["engineersIdle"] = workers - ep * engineersPerTeam;
        int components = wp * hours;
        player.record["componentsProduced"] = components;
        int componentCost = components * comMatCost;
        player.record["componentsMaterialCost"] = componentCost;
        components += player.record["componentsStored"].toInt();
        int prdComCap = ep * hours;
        int prdMatCap = components / comPerPrd;
        int order = player.record["productsOrdered"].toInt();
        int products = (prdComCap > prdMatCap)?prdMatCap:prdComCap;
        products = (products > order)?order:products;
        components -= products * comPerPrd;
        player.record["componentsUsed"] = products * comPerPrd;
        player.record["componentsStored"] = components;
        player.record["componentsTotalCost"] = componentCost + components * comStrCost;
        player.record["productsProduced"] = products;
        player.record["productsMaterialCost"] = products * prdMatCost;
    }
    return 0;
}
