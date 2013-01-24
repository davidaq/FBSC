#include "fbpproduce.h"
#include "config.h"
#include "playermanager.h"

int FBPProduce::run()
{
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();
    int workersPerTeam = config.get("Workers in a component").toInt();
    int engineersPerTeam = config.get("Engineers in a product").toInt();
    int comMatCost = config.get("Material cost per component").toInt();
    int comPerPrd = config.get("Components in a product").toInt();
    int prdMatCost = config.get("Material cost per product").toInt();
    int comStrCost = config.get("Component storage cost per quarter").toInt();
    int hours = config.get("Month in a quarter").toInt()
            * config.get("Days in a month").toInt()
            * config.get("Working hours in a month").toInt();
    int hoursInComp = config.get("Hours in a component").toInt();
    int hoursInProd = config.get("Hours in a product").toInt();
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        int workers = player.record["workersHired"].toInt();
        int engineers = player.record["engineersHired"].toInt();
        int wp = workers / workersPerTeam;
        int ep = engineers / engineersPerTeam;
        player.record["workersWorking"] = QString::number(wp * workersPerTeam);
        player.record["workersIdle"] = QString::number(workers - wp * workersPerTeam);
        player.record["engineersWorking"] = QString::number(ep * engineersPerTeam);
        player.record["engineersIdle"] = QString::number(workers - ep * engineersPerTeam);
        int components = wp * (hours / hoursInComp);
        player.record["componentsProduced"] = QString::number(components);
        int componentCost = components * comMatCost;
        player.record["componentsMaterialCost"] = QString::number(componentCost);
        components += player.record["componentsStored"].toInt();
        int prdComCap = ep * (hours / hoursInProd);
        int prdMatCap = components / comPerPrd;
        int order = player.record["productsOrdered"].toInt();
        int products = (prdComCap > prdMatCap)?prdMatCap:prdComCap;
        products = (products > order)?order:products;
        components -= products * comPerPrd;
        player.record["componentsUsed"] = QString::number(products * comPerPrd);
        player.record["componentsStored"] = QString::number(components);
        int compStorageCost = components * comStrCost;
        player.record["componentsStorageTotalCost"] = QString::number(compStorageCost);
        player.record["componentsTotalCost"] = QString::number(componentCost + compStorageCost);
        player.record["productsProduced"] = QString::number(products);
        player.record["productsStored"] = QString::number(products + player.record["productsStored"].toInt());
        int prodMatCost = products * prdMatCost;
        player.record["productsMaterialCost"] = QString::number(prodMatCost);
        int qCost = player.record["qualityCost"].toInt();
        player.cash -= qCost + componentCost + compStorageCost + prodMatCost;
        player.record["qualityCostPerProduct"] = QString::number(qCost / order);
    }
    return 0;
}
