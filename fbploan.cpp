#include "fbploan.h"
#include "config.h"
#include "playermanager.h"

int FBPLoan::run() {
    PlayerManager& players = PlayerManager::getManager();
    foreach(QString name, Config::getConfig().getPlayers()) {
        Player& player = players.getPlayer(name);
        int loan = player.record["loan"].toInt();
        if(player.loan < -loan)
            loan = -player.loan;
        player.loan += loan;
        player.cash += loan;
    }
    return 0;
}
