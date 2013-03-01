#include "fbpend.h"
#include "player.h"
#include "playermanager.h"
#include "config.h"
#include <QMap>
#include <QLinkedList>

int FBPEnd::run()
{
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();
    float interestRate = config.get("Interest per quarter").toFloat() + 1;
    QMap<qint64, QLinkedList<Player*> > rank;
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);

        player.loan = interestRate * player.loan + 0.999f;

        rank[player.cash - player.loan].append(&player);

        player.record["cashAvailableForNextRound"] = QString::number(player.cash);
        player.record["netCashflow"] = QString::number(player.cash - player.record["cashBeforeDecision"].toLongLong());
        player.record["HRCost"] = QString::number(player.record["workersTotalCost"].toLongLong() + player.record["engineersTotalCost"].toLongLong());
        player.record["productionCost"] = QString::number(player.record["componentsTotalCost"].toLongLong() + player.record["productsTotalCost"].toLongLong());
        player.record["componentsStorageChange"] = QString::number(player.record["componentsStored"].toLongLong() - player.record["componentsStorageBeforeDecision"].toLongLong());
        player.record["productsStorageChange"] = QString::number(player.record["productsStored"].toLongLong() - player.record["productsStorageBeforeDecision"].toLongLong());
        player.record["storageTotalCost"] = QString::number(player.record["componentsStorageTotalCost"].toLongLong() + player.record["productsStorageTotalCost"].toLongLong());
        player.record["totalCashInflow"] = QString::number(player.record["loan"].toLongLong() + player.record["salesProfit"].toLongLong() + player.record["bonus"].toLongLong());
        player.record["totalCashOutflow"] = QString::number(player.record["payback"].toLongLong() + player.record["HRCost"].toLongLong() + player.record["productionCost"].toLongLong() + player.record["marketingCost"].toLongLong() + player.record["salesCost"].toLongLong());
    }
    Player::globalRecord["componentsUnitStorageCost"] = config.get("Component storage cost per quarter");
    Player::globalRecord["productsUnitStorageCost"] = config.get("Component storage cost per quarter");
    Player::globalRecord["marketSize"] = QString::number((qint64)(config.get("Penetration").toFloat() * config.get("Market population").toLongLong()));
    int ranking = 1;
    foreach(QLinkedList<Player*> list, rank) {
        QString cRanking = QString::number(ranking);
        foreach(Player* player, list) {
            player->record["ranking"] = cRanking;
            ranking++;
        }
    }
    return 0;
}
