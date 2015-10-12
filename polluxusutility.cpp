#include "polluxusutility.h"

#include <QMutexLocker>
#include <QMetaType>
#include <QMap>

#include "marketdata.h"



long PolluxusUtility::baseValidId =  static_cast<long int> (time(NULL));
QMutex PolluxusUtility::mutex;

long PolluxusUtility::getNextValidId()
{
    QMutexLocker locker(&mutex);
    return baseValidId++;
}

void PolluxusUtility::registerMetaType()
{

    qRegisterMetaType<Tick>("Tick");
    qRegisterMetaType<Tick>("Tick&");
    qRegisterMetaType<Tick>("Trade");
    qRegisterMetaType<Tick>("Trade&");
    qRegisterMetaType<Depth>("Depth");
    qRegisterMetaType<Depth>("Depth&");
    qRegisterMetaType<ContractInfo>("ContractInfo");
    qRegisterMetaType<ContractInfo>("ContractInfo&");
    qRegisterMetaType<OrderBook>("OrderBook");
    qRegisterMetaType<OrderBook>("OrderBook&");
    qRegisterMetaType<QMap<QString,QStringList>>("QMap<QString,QStringList>");
    qRegisterMetaType<QMap<QString,QStringList>>("QMap<QString,QStringList>&");
    qRegisterMetaType<QMap<QString,ContractInfo>>("QMap<QString,ContractInfo>");
    qRegisterMetaType<QMap<QString,ContractInfo>>("QMap<QString,ContractInfo>&");

    qRegisterMetaType<PairOrder>("PairOrder");
    qRegisterMetaType<PairOrder>("PairOrder&");
    qRegisterMetaType<VanillaTrade>("VanillaTrade");
    qRegisterMetaType<VanillaTrade>("VanillaTrade&");
}
