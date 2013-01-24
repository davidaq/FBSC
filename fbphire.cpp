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
        int workersSalary = player.record["workersSalary"].toInt() * 3;
        player.record["workersTotalCost"] = QString::number(workersOrdered * workersSalary);
        player.cash -= workersOrdered * workersSalary;
        workersAverageSalary += workersSalary;
    }
    workersAverageSalary /= playerList.size();
    Player::globalRecord["workersAverageSalary"] = QString::number(workersAverageSalary);

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int workersOrdered = player.record["workersOrdered"].toInt();
        int workersSalary = player.record["workersSalary"].toInt() * 3;
        double m = static_cast<double>(workersAverageSalary);
#define sq(x) (x)*(x)
        int workersHired = workersAverageSalary == 0 ? 0 : (workersSalary >= workersAverageSalary ?
                    workersOrdered :  ((int) (workersOrdered * (1.0 / (sq(m)) * (double)sq(workersSalary)
                                           - 2.0 / m * workersSalary + 1.0))));
#undef sq
        player.record["workersHired"] = QString::number(workersHired);
        player.record["workersLeft"] = QString::number(workersOrdered - workersHired);
        player.record["workersAvailable"] = QString::number(workersHired);
    }

    int engineersAverageSalary = 0;
    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int engineersOrdered = player.record["engineersOrdered"].toInt();
        int engineersSalary = player.record["engineersSalary"].toInt() * 3;
        player.record["engineersTotalCost"] = QString::number(engineersOrdered * engineersSalary);
        player.cash -= engineersOrdered * engineersSalary;
        engineersAverageSalary += engineersSalary;
    }
    engineersAverageSalary /= playerList.size();
    Player::globalRecord["engineersAverageSalary"] = engineersAverageSalary;

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        int engineersOrdered = player.record["engineersOrdered"].toInt();
        int engineersSalary = player.record["engineersSalary"].toInt() * 3;
        double m = static_cast<double>(engineersAverageSalary);
#define sq(x) (x)*(x)
        int engineersHired = engineersAverageSalary == 0 ? 0 : (engineersSalary >= engineersAverageSalary ?
                    engineersOrdered :  ((int) (engineersOrdered * (1.0 / (sq(m)) * (double)sq(engineersSalary)
                                           - 2.0 / m * engineersSalary + 1.0))));
#undef sq
        player.record["engineersHired"] = QString::number(engineersHired);
        player.record["engineersLeft"] = QString::number(engineersOrdered - engineersHired);
        player.record["engineersAvailable"] = QString::number(engineersHired);
    }
    return 0;
}
