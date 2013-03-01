#include "fbpproduce.h"
#include "config.h"
#include "playermanager.h"

int FBPProduce::run()
{
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();
    qint64 workersPerTeam = config.get("Workers in a component").toLongLong();
    qint64 engineersPerTeam = config.get("Engineers in a product").toLongLong();
    qint64 comMatCost = config.get("Material cost per component").toLongLong();
    qint64 comPerPrd = config.get("Components in a product").toLongLong();
    qint64 prdMatCost = config.get("Material cost per product").toLongLong();
    qint64 comStrCost = config.get("Component storage cost per quarter").toLongLong();
    qint64 hours = config.get("Month in a quarter").toLongLong()
            * config.get("Days in a month").toLongLong()
            * config.get("Working hours in a month").toLongLong();
    qint64 hoursInComp = config.get("Hours in a component").toLongLong();
    qint64 hoursInProd = config.get("Hours in a product").toLongLong();
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        qint64 workers = player.record["workersHired"].toLongLong();
        qint64 engineers = player.record["engineersHired"].toLongLong();
        qint64 wp = workers / workersPerTeam;
        qint64 ep = engineers / engineersPerTeam;
        player.record["workersWorking"] = QString::number(wp * workersPerTeam);
        player.record["workersIdle"] = QString::number(workers % workersPerTeam);
        player.record["engineersWorking"] = QString::number(ep * engineersPerTeam);
        player.record["engineersIdle"] = QString::number(engineers % engineersPerTeam);
        qint64 components = wp * (hours / hoursInComp);
        player.record["componentsProduced"] = QString::number(components);
        qint64 componentCost = components * comMatCost;
        player.record["componentsMaterialCost"] = QString::number(componentCost);
        components += player.record["componentsStored"].toLongLong();
        qint64 prdComCap = ep * (hours / hoursInProd);
        qint64 prdMatCap = components / comPerPrd;
        qint64 order = player.record["productsOrdered"].toLongLong();
        qint64 products = (prdComCap > prdMatCap)?prdMatCap:prdComCap;
        products = (products > order)?order:products;
        components -= products * comPerPrd;
        player.record["componentsUsed"] = QString::number(products * comPerPrd);
        player.record["componentsStored"] = QString::number(components);
        qint64 compStorageCost = components * comStrCost;
        player.record["componentsStorageTotalCost"] = QString::number(compStorageCost);
        player.record["componentsTotalCost"] = QString::number(componentCost + compStorageCost);
        player.record["productsProduced"] = QString::number(products);
        player.record["productsStored"] = QString::number(products + player.record["productsStored"].toLongLong());
        qint64 prodMatCost = products * prdMatCost;
        player.record["productsMaterialCost"] = QString::number(prodMatCost);
        qint64 qCost = player.record["qualityCost"].toLongLong();
        player.cash -= qCost + componentCost + compStorageCost + prodMatCost;
        player.record["qualityCostPerProduct"] = QString::number((float) qCost / order);
    }
    return 0;
}
