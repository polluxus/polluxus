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

struct VanillaOrder
{
    QString orderId;
    QString contractId;
    QString clientId;
    QString exchange;
    QString symbol;
    QString expiry;
    QString currency;
    QString action;
    QString totalQuantity;
    QString orderType;
    QString lmtPrice;
    QString remainingQuantity;
    QString createTimeStamp;
    QString memo;
};


struct PairOrder
{
    QString orderId;
    QString symbol;
    QString action;
    QString totalQuantity;
    QString orderType;
    QString priority;
    VanillaOrder *pOrderLeg1;
    VanillaOrder *pOrderLeg2;
    QString createTimeStamp;
    QString memo;
};


struct VanillaTrade
{
    QString orderId;
    QString contractId;
    QString clientId;
    QString exchange;
    QString symbol;
    QString expiry;
    QString currency;
    QString action;
    QString quantity;
    QString price;
    QString tradeTimeStamp;
    QString memo;
};

Q_DECLARE_METATYPE(Tick)
Q_DECLARE_METATYPE(Trade)
Q_DECLARE_METATYPE(Depth)
Q_DECLARE_METATYPE(ContractInfo)
Q_DECLARE_METATYPE(OrderBook)
Q_DECLARE_METATYPE(VanillaOrder)
Q_DECLARE_METATYPE(PairOrder)
Q_DECLARE_METATYPE(VanillaTrade)

#endif // MARKETDATA

