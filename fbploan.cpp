#include "fbploan.h"
#include "config.h"
#include "playermanager.h"

FBPLoan::FBPLoan()
{
}

int FBPLoan::run() {
    PlayerManager players = PlayerManager::getManager();
    foreach(QString name, Config::getConfig().getPlayers()) {
        Player player = players.getPlayer(name);
    }
    return 0;
}
