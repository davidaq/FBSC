#include "fbploan.h"
#include "config.h"
#include "playermanager.h"

FBPLoan::FBPLoan()
{
}

int FBPLoan::run() {
    foreach(QString name, Config::getConfig().getPlayers()) {

    }

    return 0;
}
