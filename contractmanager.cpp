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

void ContractManager::test1()
{
    qDebug() << "ContractManager::test1() in thread:"  << QThread::currentThreadId();
}
void ContractManager::test2()
{
    qDebug() << "ContractManager::test2() in thread:"  << QThread::currentThreadId();
}

