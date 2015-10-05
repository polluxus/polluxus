#include "ordermanager.h"

OrderManager::OrderManager(QObject *parent) : QObject(parent)
{
    pThread = new QThread;
    pThread->start();
    moveToThread( pThread );
}

OrderManager::~OrderManager()
{
    pThread->exit( 0 );
    pThread->wait();
    delete pThread;
}

//OrderManager::newOrder()
//{

//}

//OrderManager::cancelOrder()
//{

//}

//OrderManager::modifyOrder()
//{

//}



