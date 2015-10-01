#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QObject>

struct ContractInfo;


class ContractManager : public QObject
{
    Q_OBJECT
public:
    explicit ContractManager(QObject *parent = 0);
    virtual ~ContractManager();

    QThread *pThread;
signals:
    void SubscribeMarketData(QString contractId, QString exchange);
public slots:
    void onContractDetailUpdating(const ContractInfo &contractInfo);
    void test1();
    void test2();
};

#endif // CONTRACTMANAGER_H
