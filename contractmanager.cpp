#include "contractmanager.h"
#include "marketdata.h"
#include <QDebug>
#include <QThread>
#include <QList>


ContractManager::ContractManager(QObject *parent) : QObject(parent)
{
    pThread = new QThread;
    pThread->start();
    moveToThread( pThread );
}

ContractManager::~ContractManager()
{
    pThread->exit( 0 );
    pThread->wait();
    delete pThread;
}

ContractInfo ContractManager::getContractInfoBySymbol(QString symbol)
{
    return mapContractInfo[symbol];
}

void ContractManager::onContractRetrieved(const QMap<QString, ContractInfo> &mapContractInfo)
{
    qDebug() << "ContractManager::onContractRetrieved():";
    this->mapContractInfo = mapContractInfo;
    foreach(ContractInfo contractInfo, mapContractInfo)
    {
        mapConIdSymbol[contractInfo.contractId] = contractInfo.pSymbol;
    }
}



void ContractManager::onAdapterTraded(const Trade &trade)
{
    //qDebug() << "ContractManager: onAdapterTraded() in thread: " << QThread::currentThreadId();
    qDebug() <<"ContractManager::onAdapterTraded():"
             <<trade.contractId<<"-"
             <<trade.timeStamp<<"-"
             <<trade.price<<"-"
             <<trade.size;
}

void ContractManager::onAdapterDepthed(const Depth &depth)
{
    qDebug() <<"ContractManager::onAdapterDepthed():"
             <<depth.contractId<<"-"
             <<depth.timeStamp;
}

void ContractManager::onAdapterTicked(const Tick &tick)
{
    qDebug() << "ContractManager: onAdapterTicked() in thread: " << QThread::currentThreadId();
    qDebug() <<"ContractManager::onAdapterTicked():"
             <<tick.contractId<<"-"
             <<tick.timeStamp <<"-";


    QString contractId = tick.contractId;
    if(mapConIdSymbol.contains(contractId))
    {
        Tick newTick = tick;
        newTick.contractId = mapConIdSymbol[contractId];
        emit InstrumentTicked(newTick);
    }
}

void ContractManager::onSubscribeMktData(QString symbol)
{
    //qDebug() << "ContractManager: onSubscribeMktData() in thread: " << QThread::currentThreadId();
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit ReqMktData(contractInfo.contractId, contractInfo.exchange);

        if(!mapInstrument.contains(symbol))
        {
            Instrument *pInstrument = new Instrument(&contractInfo);
            mapInstrument[symbol] = pInstrument;
        }
    }
}

void ContractManager::onSubscribeMktDepth(QString symbol)
{
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit ReqMktDepth(contractInfo.contractId, contractInfo.exchange);
    }
}

void ContractManager::onUnsubscribeMktData(QString symbol)
{
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit CancelMktData(contractInfo.contractId);
    }
}

void ContractManager::onUnsubscribeMktDepth(QString symbol)
{
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit CancelMktDepth(contractInfo.contractId);
    }

}

 void ContractManager::onReqContractInfo(QString symbol)
 {

     if(mapContractInfo.contains(symbol))
     {
         emit UpdateContractInfo(mapContractInfo[symbol]);
     }
     else
     {
         emit ReqContractInfoErr(symbol);
     }
 }
