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

void ContractManager::onContractRetrieved(QMap<QString, ContractInfo> mapContractInfo)
{
    qDebug() << "ContractManager::onContractRetrieved():";
    this->mapContractInfo = mapContractInfo;
    foreach(ContractInfo contractInfo, mapContractInfo)
    {
        mapConIdSymbol[contractInfo.contractId] = contractInfo.pSymbol;
    }
}


void ContractManager::onAdapterTraded(Trade trade)
{
    qDebug() <<"ContractManager::onAdapterTraded():"
             <<trade.contractId<<"-"
             <<trade.timeStamp<<"-"
             <<trade.price<<"-"
             <<trade.size;
}

void ContractManager::onAdapterDepthed(Depth depth)
{
    qDebug() <<"ContractManager::onAdapterDepthed():"
             <<depth.contractId<<"-"
             <<depth.timeStamp;
}

void ContractManager::onAdapterTicked(Tick tick)
{
    qDebug() <<"ContractManager::onAdapterTicked():"
             <<tick.contractId<<"-"
             <<tick.timeStamp;

    QString contractId = tick.contractId;
    if(mapConIdSymbol.contains(contractId))
    {
        tick.contractId = mapConIdSymbol[contractId];
        emit InstrumentTicked(tick);
    }
}

void ContractManager::onSubscribeMktData(QString symbol)
{
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit ReqMktData(contractInfo.contractId, contractInfo.exchange);
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

void ContractManager::onUnSubscribeMktData(QString symbol)
{
    if(mapContractInfo.contains(symbol))
    {
        ContractInfo contractInfo = mapContractInfo[symbol];
        emit CancelMktData(contractInfo.contractId);
    }
}

void ContractManager::onUnSubscribeMktDepth(QString symbol)
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
