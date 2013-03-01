#include "fbphire.h"
#include "playermanager.h"
#include "config.h"

int FBPHire::run()
{
    PlayerManager& pm = PlayerManager::getManager();
    Config& config = Config::getConfig();
    QStringList playerList = config.getPlayers();

    qint64 workersAverageSalary = 0, count = 0;
    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        qint64 workersOrdered = player.record["workersOrdered"].toLongLong();
        qint64 workersSalary = player.record["workersSalary"].toLongLong() * 3;
        player.record["workersTotalCost"] = QString::number(workersOrdered * workersSalary);
        player.cash -= workersOrdered * workersSalary;
        count += workersOrdered;
        if(count > 0)
            workersAverageSalary += (workersSalary - workersAverageSalary) * workersOrdered / count;
    }
    Player::globalRecord["workersAverageSalary"] = QString::number(workersAverageSalary);

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        qint64 workersOrdered = player.record["workersOrdered"].toLongLong();
        qint64 workersSalary = player.record["workersSalary"].toLongLong() * 3;
        double m = static_cast<double>(workersAverageSalary);
#define sq(x) (x)*(x)
        qint64 workersHired = workersAverageSalary == 0 ? 0 : (workersSalary >= workersAverageSalary ?
                    workersOrdered :  ((qint64) (workersOrdered * (1.0 / (sq(m)) * sq(workersSalary)
                                           - 2.0 / m * workersSalary + 1.0))));
        player.record["workersHired"] = QString::number(workersHired);
        player.record["workersLeft"] = QString::number(workersOrdered - workersHired);
        player.record["workersAvailable"] = QString::number(workersHired);
    }

    qint64 engineersAverageSalary = 0; count = 0;
    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        qint64 engineersOrdered = player.record["engineersOrdered"].toLongLong();
        qint64 engineersSalary = player.record["engineersSalary"].toLongLong() * 3;
        player.record["engineersTotalCost"] = QString::number(engineersOrdered * engineersSalary);
        player.cash -= engineersOrdered * engineersSalary;
        count += engineersOrdered;
        if(count > 0)
            engineersAverageSalary += (engineersSalary - engineersAverageSalary) * engineersOrdered / count;
    }
    Player::globalRecord["engineersAverageSalary"] = QString::number(engineersAverageSalary);

    foreach(QString playername, playerList)
    {
        Player& player = pm[playername];
        qint64 engineersOrdered = player.record["engineersOrdered"].toLongLong();
        qint64 engineersSalary = player.record["engineersSalary"].toLongLong() * 3;
        double m = static_cast<double>(engineersAverageSalary);
        qint64 engineersHired = engineersAverageSalary == 0 ? 0 : (engineersSalary >= engineersAverageSalary ?
                    engineersOrdered :  ((qint64) (engineersOrdered * (1.0 / (sq(m)) * (double)sq(engineersSalary)
                                           - 2.0 / m * engineersSalary + 1.0))));
#undef sq
        player.record["engineersHired"] = QString::number(engineersHired);
        player.record["engineersLeft"] = QString::number(engineersOrdered - engineersHired);
        player.record["engineersAvailable"] = QString::number(engineersHired);
    }
    return 0;
}
