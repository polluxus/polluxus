#include "contractmanager.h"
#include "marketdata.h"
#include <QDebug>
#include <QThread>

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
    qDebug() << "ContractManager::onContractDetailUpdating() in thread:"  << QThread::currentThreadId();
    qDebug() << "onContractDetailUpdating():" << contractInfo.symbol << ":" << contractInfo.exchange;
}

void ContractManager::onTickUpdating(const Tick &tick)
{
    qDebug() << "oContractManager::onTickUpdating():" << tick.contractId << ":" << tick.price<< ":" << tick.size;
}

void ContractManager::onSubscribeMktData(QString contractId, QString exchange)
{
    emit ReqMktData(contractId, exchange);
}

void ContractManager::onSubscribeMktDepth(QString contractId, QString exchange)
{
    emit ReqMktDepth(contractId, exchange);
}

void ContractManager::onUnsubscribeMktData(QString contractId)
{
    emit CancelMktData(contractId);
}

void ContractManager::onUnsubscribeMktDepth(QString contractId)
{
    emit CancelMktDepth(contractId);
}

void ContractManager::test1()
{
    qDebug() << "ContractManager::test1() in thread:"  << QThread::currentThreadId();
}
void ContractManager::test2()
{
    qDebug() << "ContractManager::test2() in thread:"  << QThread::currentThreadId();
}

