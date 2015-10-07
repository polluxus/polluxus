#include "instrument.h"
#include <QThread>
#include <QDebug>


Instrument::Instrument(ContractInfo *pContractInfo, QObject *parent) : QObject(parent)
{
    this->pContractInfo = pContractInfo;
    pOrderBook = new OrderBook();
    pLastTrade = new Trade();
}

Instrument::~Instrument()
{
    if(!pOrderBook) delete pOrderBook;
    if(!pLastTrade) delete pLastTrade;

}


