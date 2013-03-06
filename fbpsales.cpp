#include "fbpsales.h"
#include "playermanager.h"
#include "config.h"
#include <QList>
#include <QHash>

class PlayerFactor {
public:
    float price, power, quality;
};

class MarketStatus {
public:
    float _minPriceFac, _maxPriceFac;
    float _minQualityFac, _maxQualityFac;
    float _minPowerFac, _maxPowerFac;
    float _total;
    MarketStatus():_minPriceFac(0), _maxPriceFac(0),
            _minQualityFac(0), _maxQualityFac(0),
            _minPowerFac(0), _maxPowerFac(0),
            _total(0) {}

    void apply(const float & factor) {
        _total += factor;
    }
    void normalize(float & factor) {
        if(_total != 0)
            factor /= _total;
        else
            factor = 0;
    }
    void apply(const PlayerFactor & factor) {
        _apply(factor.price, _minPriceFac, _maxPriceFac);
        _apply(factor.quality, _minQualityFac, _maxQualityFac);
        _apply(factor.power, _minPowerFac, _maxPowerFac);
    }
    void normalize(PlayerFactor & factor) {
        _normalize(factor.price, _minPriceFac, _maxPriceFac);
        _normalize(factor.quality, _minQualityFac, _maxQualityFac);
        _normalize(factor.power, _minPowerFac, _maxPowerFac);
    }

private:
    static void _apply(float val, float& min,float& max) {
        if(val > max) {
            max = val;
        } else if(val < min) {
            min = val;
        }
    }
    static void _normalize(float& val, const float&  min, const float&  max) {
        if(max != min)
            val =  (val - min) / (max - min);
        else
            val = 0;
    }
};

inline qint64 pair(qint64 a, qint64 b)
{
    return a * 0x10000 + b;
}

int FBPSales::run()
{
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();

    qint64 marketSize = (qint64)(config.get("Penetration").toFloat() * config.get("Market population").toLongLong());
    qint64 addAgentCost = config.get("Adding agent cost").toLongLong();
    qint64 removeAgentCost = config.get("Removing agent cost").toLongLong();

    QHash<int, MarketStatus> marketStatus;
    QHash<int, PlayerFactor> factors;

    qint64 playerIndex = -1;
    foreach(QString name, config.getPlayers()) {
        playerIndex++;
        Player& player = manager.getPlayer(name);
        // Add / Remove agent
        qint64 addCost = 0, removeCost = 0;
        int addCount = 0, removeCount = 0;
        int total = 0;
        for(int i = 0, sz = manager.playerCount(); i < sz; i++) {
            QString s = QString::number(i);
            player.record["agents_" + s] = player.record["agentsBeforeDecision_" + s] = QString::number(player.marketAgents[i]);
            player.record["agentsChangeCost_" + s] = "0";
            total += player.marketAgents[i];
        }
        player.record["agentsBeforeDecision_total"] = QString::number(total);
        if(!player.record["saleAgentAdded"].isEmpty())
            foreach(QString s, player.record["saleAgentAdded"].split(";;")) {
                int marketid = s.toInt();
                if(player.marketAgents.contains(marketid)) {
                    player.marketAgents[marketid] += 1;
                } else {
                    player.marketAgents[marketid] = 1;
                }
                player.record["agentsAdd_" + s] = "1";
                player.record["agentsChangeCost_" + s] = QString::number(addAgentCost);
                player.record["agents_" + s] = QString::number(player.marketAgents[marketid]);
                addCount++;
            }
        addCost = addCount * addAgentCost;
        if(!player.record["saleAgentRemoved"].isEmpty())
            foreach(QString s, player.record["saleAgentRemoved"].split(";;")) {
                int marketid = s.toInt();
                if(player.homeMarket == marketid && player.marketAgents[marketid] == 1) {
                    continue;
                }
                if(player.marketAgents.contains(marketid)) {
                    player.marketAgents[marketid] -= 1;
                    player.record["agentsRemove_" + s] = "1";
                    player.record["agentsChangeCost_" + s] = QString::number(player.record["agentsChangeCost_" + s].toLongLong() + removeAgentCost);
                    player.record["agents_" + s] = QString::number(player.marketAgents[marketid]);
                    removeCount++;
                }
            }
        removeCost = removeCount * removeAgentCost;
        player.record["agentsAdd_total"] = QString::number(addCount);
        player.record["agentsRemove_total"] = QString::number(removeCount);
        player.record["saleAgentsCost"] = QString::number(addCost + removeCost);
        player.record["agents_total"] = QString::number(total - removeCount + addCount);
        qint64 totalAgent = 0;
        foreach(qint64 k, player.marketAgents.keys()) {
            qint64 n = player.marketAgents[k];
            totalAgent += n;
        }
        PlayerFactor factor;
        qint64 salesSupport = player.record["salesSupport"].toLongLong();
        player.record["salesCost"] = QString::number(addCost + removeCost + salesSupport);
        player.cash -= addCost + removeCost + salesSupport;
        float singlePower = (totalAgent > 0) ? (float) salesSupport / totalAgent : 0;
        player.record["salesSupportPerAgent"] = QString::number(singlePower);
        factor.price = -player.record["price"].toLongLong();
        factor.quality = player.record["qualityCostPerProduct"].toFloat();
        foreach(int k, player.marketAgents.keys()) {
            int n = player.marketAgents[k];
            if(n > 0) {
                PlayerFactor factor2 = factor;
                factor2.power = singlePower * n;
                player.record["salesSupport_" + QString::number(k)] = QString::number(singlePower * n);
                marketStatus[k].apply(factor2);
                factors[pair(playerIndex, k)] = factor2;
            }
        }
    }
    QHash<int, float> totalFactors;
    playerIndex = -1;
    foreach(QString name, config.getPlayers()) {
        playerIndex++;
        Player& player = manager.getPlayer(name);

        foreach(qint64 k, player.marketAgents.keys()) {
            qint64 n = player.marketAgents[k];
            if(n > 0) {
                qint64 key = pair(playerIndex, k);
                PlayerFactor factor = factors[key];
                marketStatus[k].normalize(factor);
                float total = factor.power + factor.price + factor.quality;
                marketStatus[k].apply(total);
                totalFactors[key] = total;
            }
        }
    }
    factors.clear();
    playerIndex = -1;
    int reportCost = config.get("Market report cost per market quarter").toInt();
    int consultantPrice = config.get("Consultant cost per 30 minutes").toInt();
    qint64 prdStrCost = config.get("Product storage cost per quarter").toLongLong();
    Player::globalRecord["marketReportPrice"] = QString::number(reportCost);
    foreach(QString name, config.getPlayers()) {
        playerIndex++;
        Player& player = manager.getPlayer(name);

        qint64 totalOrder = 0;
        qint64 price = player.record["price"].toLongLong();
        foreach(qint64 k, player.marketAgents.keys()) {
            qint64 n = player.marketAgents[k];
            if(n > 0) {
                qint64 key = pair(playerIndex, k);
                float factor = totalFactors[key];
                marketStatus[k].normalize(factor);
                totalFactors[key] = factor;
                player.record["marketShare_" + QString::number(k)] = QString::number(factor);
                qint64 order = factor * marketSize;
                player.record["marketOrder_" + QString::number(k)] = QString::number(order);
                totalOrder += order;
            }
        }
        player.record["totalReceivedOrders"] = QString::number(totalOrder);
        qint64 stored = player.record["productsStored"].toLongLong();
        qint64 satisfied = (stored > totalOrder) ? totalOrder : stored;
        player.record["totalSatisfiedOrders"] = QString::number(satisfied);
        player.record["totalUnsatisfiedOrders"] = QString::number((satisfied < totalOrder) ? (totalOrder - satisfied) : 0);
        stored -= satisfied;
        player.record["productsStored"] = QString::number(stored);
        player.record["productsStorageTotalCost"] = QString::number(stored * prdStrCost);
        player.record["productsTotalCost"] = QString::number(player.record["qualityCost"].toLongLong() + player.record["productsMaterialCost"].toLongLong() + stored * prdStrCost);
        player.cash -= stored * prdStrCost;
        qint64 profit = satisfied * price;
        player.cash += profit;
        player.record["salesProfit"] = QString::number(profit);

        // marketing
        int orderedReport = 0;
        if(!player.record["marketsOrderedReport"].isEmpty()) {
            orderedReport = player.record["marketsOrderedReport"].split(";;").count();
        }
        player.record["marketReportOrdered"] = QString::number(orderedReport);
        int marketingCost = reportCost * orderedReport;
        player.record["marketReportCost"] = QString::number(marketingCost);
        if(player.record["hireConsultant"].toInt()) {
            marketingCost += consultantPrice;
            player.record["consultantCost"] = QString::number(consultantPrice);
        }
        player.cash -= marketingCost;
        player.record["marketingCost"] = QString::number(marketingCost);
    }
    return 0;
}
