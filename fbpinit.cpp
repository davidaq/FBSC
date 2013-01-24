#include "fbpinit.h"
#include "config.h"
#include "player.h"
#include "playermanager.h"

int FBPInit::run()
{
    PlayerManager& pm = PlayerManager::getManager();
    Config& config = Config::getConfig();
    int playerIndex = -1;
    foreach(QString playername, config.getPlayers())
    {
        playerIndex++;
        Player player;
        player.name = playername;
        player.loan = 0;
        player.cash = config.get("Initial cash").toInt();
        player.homeMarket = playerIndex;
        player.marketAgents[playerIndex] = 1;

        pm.addPlayer(player);
    }
    return 0;
}
