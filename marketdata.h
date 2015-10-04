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
    QString price;
    QString size;
};

struct Depth
{
    QString contractId;
    QDateTime timeStamp;
    int position;
    int operation;
    int side;
    double price;
    int size;
};


struct ContractInfo
{
    QString contractId;
    QString symbol;
    QString secType;
    QString exchange;
    QString primaryExchange;
    QString expiry;
    QString currency;
    int lotSize;
    double minTick;
    double multiplier;
    QString mktDataStatus = "OFF";
    QString mktDepthStatus = "OFF";
};

struct OrderBook
{
    QString contractId;
    QString timeStamp;
    int flag;   //-1: last update is bid, 0: last update is last, 1: last update is ask

    QString lastTradePx;
    QString lastTradeSz;

    QStringList lstBidPx;
    QStringList lstBidSz;
    QStringList lstAskPx;
    QStringList lstAskSz;
};



Q_DECLARE_METATYPE(Tick)
Q_DECLARE_METATYPE(Depth)
Q_DECLARE_METATYPE(ContractInfo)
Q_DECLARE_METATYPE(OrderBook)

#endif // MARKETDATA

