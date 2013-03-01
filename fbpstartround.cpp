#include "fbpstartround.h"
#include "config.h"
#include "playermanager.h"

int FBPStartRound::run() {
    PlayerManager& players = PlayerManager::getManager();
    foreach(QString name, Config::getConfig().getPlayers()) {
        Player& player = players.getPlayer(name);
        player.record["cashBeforeDecision"] = QString::number(player.cash);
        player.record["componentsStorageBeforeDecision"] = player.record["componentsStored"];
        player.record["productsStorageBeforeDecision"] = player.record["productsStored"];
    }
    Player::globalRecord["round"] = QString::number(Player::globalRecord["round"].toInt() + 1);
    return 0;
}
