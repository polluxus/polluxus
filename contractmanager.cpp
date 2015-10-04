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

void ContractManager::onContractDetailUpdating(const ContractInfo &contractInfo)
{
//    qDebug() << "ContractManager::onContractDetailUpdating() in thread:"  << QThread::currentThreadId();
//    qDebug() << "onContractDetailUpdating():" << contractInfo.symbol << ":" << contractInfo.exchange;
}

void ContractManager::onTickUpdating(const Tick &tick)
{
    qDebug() << "oContractManager::onTickUpdating():" << tick.contractId << ":" << tick.price<< ":" << tick.size;
    mapTick[tick.contractId] = tick;
}

void ContractManager::onSubscribeMktData(QString contractId, QString exchange)
{
    emit ReqMktData(contractId, exchange);
    mapContractInfo[contractId].mktDataStatus = "ON";
}

void ContractManager::onSubscribeMktDepth(QString contractId, QString exchange)
{
    emit ReqMktDepth(contractId, exchange);
    mapContractInfo[contractId].mktDepthStatus = "ON";
}

void ContractManager::onUnsubscribeMktData(QString contractId)
{
    emit CancelMktData(contractId);
    mapContractInfo[contractId].mktDataStatus = "OFF";
}

void ContractManager::onUnsubscribeMktDepth(QString contractId)
{
    emit CancelMktDepth(contractId);
    mapContractInfo[contractId].mktDepthStatus = "OFF";
}

void ContractManager::test1()
{
    qDebug() << "ContractManager::test1() in thread:"  << QThread::currentThreadId();
}
void ContractManager::test2()
{
    qDebug() << "ContractManager::test2() in thread:"  << QThread::currentThreadId();
}
void ContractManager::onContractRetrieved(QMap<QString, QStringList> mapContract)
{
    int nContracts = mapContract.size();
    if(nContracts > 0)
    {
        int idx;
        mapContractInfo.clear();
        QList<QString> mKeyList = mapContract.keys();

        for(idx = 0; idx < nContracts; idx++)
        {
            QString contractId = mKeyList[idx];

            mapContractInfo[contractId].contractId = contractId;
            mapContractInfo[contractId].exchange = mapContract[contractId][1];
            mapContractInfo[contractId].symbol = mapContract[contractId][2];
            mapContractInfo[contractId].secType = mapContract[contractId][3];
            mapContractInfo[contractId].expiry = mapContract[contractId][4];
            mapContractInfo[contractId].lotSize = (mapContract[contractId][5]).toInt();
            mapContractInfo[contractId].multiplier = (mapContract[contractId][6]).toDouble();

            mapContractInfo[contractId].mktDataStatus = "OFF";
            mapContractInfo[contractId].mktDepthStatus = "OFF";
        }
    }

}
