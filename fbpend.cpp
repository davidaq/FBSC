#include "fbpend.h"
#include "player.h"
#include "playermanager.h"
#include "config.h"

int FBPEnd::run()
{
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();
    int prdStrCost = config.get("Product storage cost per quarter").toInt();
    float interestRate = config.get("Interest per quarter").toFloat() + 1;
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        int prodStoreCost = player.record["productsStored"].toInt() * prdStrCost;
        player.cash -= prodStoreCost;
        player.record["productsStorageTotalCost"] = QString::number(player.record["productsMaterialCost"].toInt() + prodStoreCost);
        player.record["loan"] = QString::number((int)(interestRate * player.record["loan"].toInt() + 0.999f));
    }
    return 0;
}
