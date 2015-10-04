#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QObject>
#include <QMap>

struct ContractInfo;
struct Tick;
struct OrderBook;



class ContractManager : public QObject
{
    Q_OBJECT
public:
    explicit ContractManager(QObject *parent = 0);
    virtual ~ContractManager();

    QThread *pThread;

    QMap<QString, Tick> mapTick;
    QMap<QString, ContractInfo> mapContractInfo;
    QMap<QString, OrderBook> mapOrderBook;

signals:
    void ReqMktData(QString contractId, QString exchange);
    void ReqMktDepth(QString contractId, QString exchange);
    void CancelMktData(QString contractId);
    void CancelMktDepth(QString contractId);

public slots:
    void onTickUpdating(const Tick &tick);
    void onContractDetailUpdating(const ContractInfo &contractInfo);

    void onSubscribeMktData(QString contractId, QString exchange);
    void onSubscribeMktDepth(QString contractId, QString exchange);
    void onUnsubscribeMktData(QString contractId);
    void onUnsubscribeMktDepth(QString contractId);
    void onContractRetrieved(QMap<QString, QStringList> mapContract);

    void test1();
    void test2();
};

#endif // CONTRACTMANAGER_H
