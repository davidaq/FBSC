#include "fbpinit.h"
#include "config.h"
#include "player.h"
#include "playermanager.h"

FBPInit::FBPInit()
{
}

FBPInit::~FBPInit()
{

}

int FBPInit::run()
{
    PlayerManager pm = PlayerManager.getManager();
    Config config = Config.getConfig;
    config.getMarket();
    QStringList playerList = config.getPlayers();
    foreach(QString player, playerList)
    {
        Player player;
        player.name = player;
        player.loan = 0;
        player.cash = config.get("Initial cash").toInt();


        PlayerManager.addPlayer();
    }
}

