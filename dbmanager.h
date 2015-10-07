#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QStringList>
#include "marketdata.h"


class QThread;

class DbManager  : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(const QString& path, QObject *parent = 0);
    virtual ~DbManager();

    bool isOpen() const;
    bool addContract(QString pSymbol, QString contractId, QString symbol, QString localSymbol,  QString secType,
                     QString exchange, QString primaryExchange, QString expiry,
                     QString currency, QString lotSize,QString minTick, QString multiplier);
    bool removeContract(QString pSymbol);
    bool contractExists(QString pSymbol);
    void printAllContracts();
    bool removeAllContracts();

    QThread *pThread;

signals:
    void ContractRetrieved(QMap<QString, ContractInfo> mapContractInfo);

public slots:
    void onRetrieveContract();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H



