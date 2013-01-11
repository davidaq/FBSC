#include "fbpinit.h"
#include "config.h"
#include "player.h"
#include "playermanager.h"

int FBPInit::run()
{
    PlayerManager pm = PlayerManager::getManager();
    Config config = Config::getConfig();
    QStringList playerList = config.getPlayers();
    foreach(QString playername, playerList)
    {
        Player player;
        player.name = playername;
        player.loan = 0;
        player.cash = config.get("Initial cash").toInt();

        pm.addPlayer(player);
    }
    return 0;
}
