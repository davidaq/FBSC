#include "fbploan.h"
#include "config.h"
#include "playermanager.h"

int FBPLoan::run() {
    PlayerManager& players = PlayerManager::getManager();
    foreach(QString name, Config::getConfig().getPlayers()) {
        Player& player = players.getPlayer(name);
        qint64 loan = player.record["loan"].toLongLong();
        if(loan < 0) {
            player.record["payback"] = QString::number(-loan);
            player.record["loan"] = "0";
            if(player.loan < -loan)
                loan = -player.loan;
        }
        player.loan += loan;
        player.cash += loan;
    }
    return 0;
}
