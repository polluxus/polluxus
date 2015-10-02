#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QObject>

struct ContractInfo;
struct Tick;


class ContractManager : public QObject
{
    Q_OBJECT
public:
    explicit ContractManager(QObject *parent = 0);
    virtual ~ContractManager();

    QThread *pThread;

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

    void test1();
    void test2();
};

#endif // CONTRACTMANAGER_H
