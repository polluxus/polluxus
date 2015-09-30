#ifndef MARKETDATA
#define MARKETDATA

#include <QString>
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


struct TickerData
{
    QString contractId;
    QString symbol;
    QString secType;
    QString exchange;
    QString expiry;
    QString currency;
    int lotSize;
    double minTick;
    double multiplier;
};



Q_DECLARE_METATYPE(Tick)
Q_DECLARE_METATYPE(Depth)
Q_DECLARE_METATYPE(TickerData)

#endif // MARKETDATA

