#include "ordermanager.h"
#include "polluxusutility.h"

OrderManager::OrderManager(ContractManager *pContractManager, QObject *parent) : QObject(parent)
{
    this->pContractManager = pContractManager;

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

void OrderManager::onOrderSubmit(QString symbol, QString action, QString orderType, double price, int qty, QString strategyId)
{
    ContractInfo contractInfo = pContractManager->getContractInfoBySymbol(symbol);
    OrderItem orderItem;
    orderItem.orderId = PolluxusUtility::getNextValidId();
    orderItem.pSymbol = symbol;
    orderItem.contractId = contractInfo.contractId;
    orderItem.exchange = contractInfo.exchange;
    orderItem.action = action;
    orderItem.orderType = orderType;
    orderItem.lmtPrice = price;
    orderItem.totalQuantity = qty;
    orderItem.strategyId = strategyId;
    orderItem.orderStatus = "submitting";
    orderItem.remainingQuantity = qty;
    orderItem.filled = 0;
    orderItem.createTimeStamp = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

    if(!mapOrderItem.contains(orderItem.orderId))
    {
        mapOrderItem[orderItem.orderId] = orderItem;
        emit NewOrder(orderItem);

    }
    else
    {
        //sth wrong here, shoud handle this later
    }

}

void OrderManager::onModifyOrder(OrderItem orderItem)
{

}

void OrderManager::onCancelOrder(OrderItem orderItem)
{

}

void OrderManager::onOrderUpdated(const OrderUpdate &orderUpdate)
{
    if(mapOrderItem.contains(orderUpdate.orderId))
    {
        //int lastFilled = 0;

        OrderItem *pOrderItem = &mapOrderItem[orderUpdate.orderId];
        pOrderItem->orderStatus = orderUpdate.status;
        //lastFilled = orderUpdate.filled - pOrderItem->filled;
        pOrderItem->filled = orderUpdate.filled;
        pOrderItem->remainingQuantity = orderUpdate.remaining;
        pOrderItem->avgFillPrice = orderUpdate.avgFillPrice;
        pOrderItem->lastFillPrice = orderUpdate.lastFillPrice;

        emit OrderItemUpdated(*pOrderItem);
    }

}


