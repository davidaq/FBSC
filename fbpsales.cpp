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
        factor /= _total;
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
        val =  (val - min) / (max - min);
    }
};

inline int pair(int a, int b)
{
    return a * 0x10000 + b;
}

int FBPSales::run()
{
    PlayerManager manager = PlayerManager::getManager();
    Config config = Config::getConfig();

    int marketSize = (int)(config.get("Penetration").toDouble() * config.get("Market population").toInt());
    int addAgentCost = config.get("Adding agent cost").toInt();
    int removeAgentCost = config.get("Removing agent cost").toInt();

    QHash<int, MarketStatus> marketStatus;
    QHash<int, PlayerFactor> factors;

    int playerIndex = -1;
    foreach(QString name, config.getPlayers()) {
        playerIndex++;
        Player& player = manager.getPlayer(name);
        // Add / Remove agent
        int addCost = 0, removeCost = 0;
        foreach(QString s, player.record["saleAgentAdded"].split(";;")) {
            int marketid = s.toInt();
            if(player.marketAgents.contains(marketid)) {
                player.marketAgents[marketid] += 1;
                addCost += addAgentCost;
            }
        }
        foreach(QString s, player.record["saleAgentRemoved"].split(";;")) {
            int marketid = s.toInt();
            if(player.homeMarket == marketid && player.marketAgents[marketid] == 1) {
                continue;
            }
            if(player.marketAgents.contains(marketid)) {
                player.marketAgents[marketid] -= 1;
                removeCost += removeAgentCost;
            }
        }
        player.record["saleAgentsCost"] = QString::number(addCost + removeCost);
        player.cash -= addCost + removeCost;
        int totalAgent = 0;
        foreach(int k, player.marketAgents.keys()) {
            int n = player.marketAgents[k];
            totalAgent += n;
        }
        PlayerFactor factor;
        int salesBudget = player.record["salesBudget"].toInt();
        player.cash -= salesBudget;
        float singlePower = (float) salesBudget / totalAgent;
        factor.price = -player.record["price"].toInt();
        factor.quality = player.record["qualityCostPerProduct"].toInt();
        foreach(int k, player.marketAgents.keys()) {
            int n = player.marketAgents[k];
            if(n > 0) {
                PlayerFactor factor2 = factor;
                factor2.power = singlePower * n;
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

        foreach(int k, player.marketAgents.keys()) {
            int n = player.marketAgents[k];
            if(n > 0) {
                int key = pair(playerIndex, k);
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
    foreach(QString name, config.getPlayers()) {
        playerIndex++;
        Player& player = manager.getPlayer(name);

        int totalOrder = 0;
        int price = player.record["price"].toInt();
        foreach(int k, player.marketAgents.keys()) {
            int n = player.marketAgents[k];
            if(n > 0) {
                int key = pair(playerIndex, k);
                float factor = totalFactors[key];
                marketStatus[k].normalize(factor);
                totalFactors[key] = factor;
                player.record["marketShare_"+k] = QString::number(factor);
                int order = factor * marketSize;
                player.record["marketOrder_"+k] = QString::number(order);
                totalOrder += order;
            }
            player.record["totalReceivedOrders"] = QString::number(totalOrder);
            int stored = player.record["productsStored"].toInt();
            int satisfied = (stored > totalOrder) ? totalOrder : stored;
            player.record["totalSatisfiedOrders"] = QString::number(satisfied);
            player.record["totalUnsatisfiedOrders"] = QString::number((satisfied < totalOrder) ? (totalOrder - satisfied) : 0);
            player.record["productsStored"] = QString::number(stored - satisfied);
            int profit = satisfied * price;
            player.cash += profit;
            player.record["salesProfit"] = QString::number(profit);
        }
    }
    return 0;
}
