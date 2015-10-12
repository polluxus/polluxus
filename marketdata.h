#ifndef MARKETDATA
#define MARKETDATA

#include <QString>
#include <QStringList>
#include <QList>
#include <QMetaType>
#include <QDateTime>

struct Tick
{
    QString contractId;
    QString timeStamp;
    QString typeId;
    QString value;
};

struct Trade
{
    QString contractId;
    QString timeStamp;
    QString price;
    QString size;
};

struct Depth
{
    QString contractId;
    QString timeStamp;
    QString marketMaker;
    QString position;
    QString operation;
    QString side;
    QString price;
    QString size;
};


struct ContractInfo
{
    QString pSymbol;
    QString contractId;
    QString symbol;
    QString localSymbol;
    QString secType;
    QString exchange;
    QString primaryExchange;
    QString expiry;
    QString currency;
    QString lotSize;
    QString minTick;
    QString multiplier;
};

struct OrderBook
{
    QString contractId;
    QString timeStamp;

    QStringList lstBidPx;
    QStringList lstBidSz;
    QStringList lstAskPx;
    QStringList lstAskSz;
};

struct OrderItem
{
    long orderId;
    QString pSymbol;
    QString contractId;
    QString exchange;
    QString action;
    int totalQuantity;
    QString orderType;
    double lmtPrice;
    int remainingQuantity;
    QString createTimeStamp;
    QString strategyId;
    QString orderStatus;
    QString parentOrderId;
    double avgFillPrice;
    double lastFillPrice;
    int filled;
    QString memo;
};
struct OrderUpdate
{
    long orderId;
    QString status;
    int filled;
    int remaining;
    double avgFillPrice;
    int permId;
    int parentId;
    double lastFillPrice;
    int clientId;
    QString whyHeld;

};


struct PairOrder
{
    QString orderId;
    QString symbol;
    QString action;
    QString totalQuantity;
    QString orderType;
    QString priority;
    OrderItem *pOrderLeg1;
    OrderItem *pOrderLeg2;
    QString createTimeStamp;
    QString memo;
};


struct TradeItem
{
    QString orderId;
    QString symbol;
    QString action;
    int fillQty;
    double fillPrice;
    double actualPrice;
    QString tradeTimeStamp;
};

struct TradeSummary
{
    QString symbol;
    int netQty;
    int boughtQty;
    int soldQty;
    double avgPriceBuy;
    double avgPriceSell;
    double avgActualPriceBuy;
    double avgActualPriceSell;
    double realizedPnL;
    double unRealizedPnL;
    double mtmPnl;
};

Q_DECLARE_METATYPE(Tick)
Q_DECLARE_METATYPE(Trade)
Q_DECLARE_METATYPE(Depth)
Q_DECLARE_METATYPE(ContractInfo)
Q_DECLARE_METATYPE(OrderBook)
Q_DECLARE_METATYPE(OrderItem)
Q_DECLARE_METATYPE(OrderUpdate)
Q_DECLARE_METATYPE(PairOrder)
Q_DECLARE_METATYPE(TradeItem)
Q_DECLARE_METATYPE(TradeSummary)
#endif // MARKETDATA

