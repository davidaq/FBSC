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
    int prdMatCost = config.get("Material cost per product").toInt();
    int comStrCost = config.get("Component storage cost per quarter").toInt();
    foreach(QString name, config.getPlayers()) {
        Player player = manager.getPlayer(name);
        int workers = player.record["workersHired"].toInt();
        int engineers = player.record["engineersHired"].toInt();
    }
}
