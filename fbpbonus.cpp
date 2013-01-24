#include "fbpbonus.h"
#include "player.h"
#include "playermanager.h"
#include "config.h"

int FBPBonus::run()
{
    PlayerManager manager = PlayerManager::getManager();
    Config config = Config::getConfig();
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        if(player.record.contains("bonus")) {
            player.cash += player.record["bonus"].toInt();
        }
    }
    return 0;
}
