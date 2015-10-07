#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QObject>
#include "marketdata.h"
#include <QReadWriteLock>


class Instrument : public QObject
{
    Q_OBJECT
public:
    explicit Instrument(ContractInfo *pContractInfo, QObject *parent = 0);
    virtual ~Instrument();

    ContractInfo *pContractInfo;
    OrderBook *pOrderBook;
    Trade *pLastTrade;

    void connectSignalSlot();
private:
    QReadWriteLock lock;

signals:


public slots:


};

#endif // INSTRUMENT_H
