#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QHash>

class Player
{
public:
    QString name;
    qint64 cash;
    qint64 loan;
    int homeMarket;
    QHash<int, int> marketAgents;
    QHash<QString, QString> record;
    static QHash<QString, QString> globalRecord;
    /*
    /// Key matrics
    int cashBeforeDecision;
    int netCashflow;
    int cashAvailableForNextRound;
    int ranking;

    /// Financials
    //Cash outflow
    int HRCost;
    int productionCost;
    int salesCost;
    int marketingCost;
    int totalCashOutflow;
    int netCashflowProfit;
    //Cash inflow
    int salesRevenue;
    int projectBonusRevenue;
    int totalCashInflow;
    int netCashflowLoss;

    /// HR
    // Workers
    int workersOrdered;
    int workersHired;
    int workersLeft;
    int workersAvailable;
    int workersWorking;
    int workersIdle;
    static int workersAverageSalary;
    int workersSalary;
    int workersTotalCost;
    // Engineers
    int engineersOrdered;
    int engineersHired;
    int engineersLeft;
    int engineersAvailable;
    int engineersWorking;
    int engineersIdle;
    static int engineersAverageSalary;
    int engineersSalary;
    int engineersTotalCost;

    /// Production
    // Components
    int componentsCapacity;
    int componentsProduced;
    int componentsUsed;
    int componentsStored;
    int componentsMaterialCost;
    int componentsTotalCost;
    // Products
    int productsCapacity;
    int productsOrdered;
    int productsProduced;
    int productsUsed;
    int productsStored;
    int productsMaterialCost;
    int productsTotalCost;

    /// Warehouse
    // Components
    int componentsStorageBeforeDecision;
    int componentsStorageIn;
    int componentsStorageOut;
    int componentsTotalItems;
    int componentsStorageTotalCost;
    // Products
    int productsStorageBeforeDecision;
    int productsStorageIn;
    int productsStorageOut;
    int productsTotalItems;
    int productsStorageTotalCost;

    /// Quality & R&D
    int qualityCost;
    int qualityCostPerProduct;

    /// Sales
    // Market Share
    QHash<int, double> marketShare;
    QHash<int, int> totalOrders;
    QHash<int, int> receivedOrders;
    // Revenue
    int totalReceivedOrders;
    int totalSatisfiedOrders;
    int totalUnsatisfiedOrders;
    int price;

    /// Sale Agent Costs
    QHash<int, int> saleAgentsAdded;
    QHash<int, int> saleAgentsRemoved;
    QHash<int, int> saleAgentsCost;
    float salesPower;
    int salesBudget;

    QList<int> marketsOrderedReport;
    bool hireConsultant;
    int moneyPaidForLoan;
    */
};

#endif // PLAYER_H
