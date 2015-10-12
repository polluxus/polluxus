#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include "marketdata.h"
#include "contractmanager.h"

class OrderManager : public QObject
{
    Q_OBJECT
public:
    explicit OrderManager(ContractManager *pContractManager, QObject *parent = 0);
    virtual ~OrderManager();

    QMap<long, OrderItem> mapOrderItem;
    QList<TradeItem> lstTradeItem;
    QMap<QString, TradeSummary> mapTradeSummary;

    QThread *pThread;
    ContractManager *pContractManager;

signals:
    void NewOrder(const OrderItem &orderItem);
    void ModifyOrder();
    void CancelOrder();

    void OrderItemUpdated(const OrderItem &orderItem);

public slots:
    void onNewOrder(QString symbol, QString action, QString orderType, double price, int qty, QString strategyId);
    void onModifyOrder(OrderItem orderItem);
    void onCancelOrder(OrderItem orderItem);

    void onOrderUpdated(const OrderUpdate &orderUpdate);
};

#endif // ORDERMANAGER_H
