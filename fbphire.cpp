#include "fbphire.h"
#include "playermanager.h"
#include "config.h"

int FBPHire::run()
{
    PlayerManager pm = PlayerManager::getManager();
    Config config = Config::getConfig();
    QStringList playerList = config.getPlayers();

    int workersAverageSalary = 0;
    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int workersOrdered = player.record["workersOrdered"].toInt();
        int workersSalary = player.record["workersSalary"].toInt();
        player.record["workersTotalCost"] = workersOrdered * workersSalary;
        workersAverageSalary += workersSalary;
    }
    workersAverageSalary /= playerList.size();
    Player::globalRecord["workersAverageSalary"] = workersAverageSalary;

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int workersOrdered = player.record["workersOrdered"].toInt();
        int workersSalary = player.record["workersSalary"].toInt();
        double m = static_cast<double>(workersAverageSalary);
#define sq(x) (x)*(x)
        int workersHired = workersAverageSalary == 0 ? 0 : (workersSalary >= workersAverageSalary ?
                    workersOrdered :  ((int) (workersOrdered * (1.0 / (sq(m)) * (double)sq(workersSalary)
                                           - 2.0 / m * workersSalary + 1.0))));
#undef sq
        player.record["workersHired"] = workersHired;
        player.record["workersLeft"] = workersOrdered - workersHired;
        player.record["workersAvailable"] = workersHired;
    }

    int engineersAverageSalary = 0;
    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int engineersOrdered = player.record["engineersOrdered"].toInt();
        int engineersSalary = player.record["engineersSalary"].toInt();
        player.record["engineersTotalCost"] = engineersOrdered * engineersSalary;
        engineersAverageSalary += engineersSalary;
    }
    engineersAverageSalary /= playerList.size();
    Player::globalRecord["engineersAverageSalary"] = engineersAverageSalary;

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int engineersOrdered = player.record["engineersOrdered"].toInt();
        int engineersSalary = player.record["engineersSalary"].toInt();
        double m = static_cast<double>(engineersAverageSalary);
#define sq(x) (x)*(x)
        int engineersHired = engineersAverageSalary == 0 ? 0 : (engineersSalary >= engineersAverageSalary ?
                    engineersOrdered :  ((int) (engineersOrdered * (1.0 / (sq(m)) * (double)sq(engineersSalary)
                                           - 2.0 / m * engineersSalary + 1.0))));
#undef sq
        player.record["engineersHired"] = engineersHired;
        player.record["engineersLeft"] = engineersOrdered - engineersHired;
        player.record["engineersAvailable"] = engineersHired;
    }
    return 0;
}
