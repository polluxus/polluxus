#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QThread>
#include <QDate>





DbManager::DbManager(const QString &path, QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    pThread = new QThread;
    pThread->start();
    moveToThread( pThread );


}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }

    pThread->exit( 0 );
    pThread->wait();
    delete pThread;

}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::addContract(QString pSymbol, QString contractId, QString symbol, QString localSymbol,  QString secType,
                            QString exchange, QString primaryExchange, QString expiry,
                            QString currency, QString lotSize,QString minTick, QString multiplier)
{
    bool success = false;

    if (!contractId.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO contract_ib (pSymbol, contractId, symbol, localSymbol,secType, "
                         "exchange, primaryExchange, expiry, currency, lotSize, minTick, multiplier) "
                         "VALUES (:pSymbol, :contractId, :symbol, :localSymbol, :secType, "
                         ":exchange, :primaryExchange, :expiry, :currency, :lotSize, :minTick, :multiplier)");

        queryAdd.bindValue(":pSymbol", pSymbol);
        queryAdd.bindValue(":contractId", contractId);
        queryAdd.bindValue(":symbol", symbol);
        queryAdd.bindValue(":localSymbol", localSymbol);
        queryAdd.bindValue(":secType", secType);
        queryAdd.bindValue(":exchange", exchange);
        queryAdd.bindValue(":primaryExchange", primaryExchange);
        queryAdd.bindValue(":expiry", expiry);
        queryAdd.bindValue(":currency", currency);
        queryAdd.bindValue(":lotSize", lotSize);
        queryAdd.bindValue(":minTick", minTick);
        queryAdd.bindValue(":multiplier", multiplier);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add contract failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add contract failed: contractId cannot be empty";
    }

    return success;
}

bool DbManager::removeContract(QString pSymbol)
{
    bool success = false;

    if (contractExists(pSymbol))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM contract_ib WHERE pSymbol = (:pSymbol)");
        queryDelete.bindValue(":pSymbol", pSymbol);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove contract failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove contract failed: person doesnt exist";
    }

    return success;
}

void DbManager::printAllContracts()
{
    qDebug() << "Contracts in db:";
    QSqlQuery query("SELECT * FROM contract_ib");

    while (query.next())
    {

        QString pSymbol = query.value("pSymbol").toString();
        QString contractId = query.value("contractId").toString();
        QString symbol = query.value("symbol").toString();
        QString localSymbol = query.value("localSymbol").toString();
        QString secType = query.value("secType").toString();
        QString exchange = query.value("exchange").toString();
        QString primaryExchange = query.value("primaryExchange").toString();
        QString expiry = query.value("expiry").toString();
        QString currency = query.value("currency").toString();
        QString lotSize = query.value("lotSize").toString();
        QString minTick = query.value("minTick").toString();
        QString multiplier = query.value("multiplier").toString();

        qDebug() << "===" << pSymbol <<"-"
                          << contractId <<"-"
                          << exchange <<"-"
                          << symbol <<"-"
                          << secType <<"-"
                          << expiry <<"-"
                          << lotSize <<"-"
                          << multiplier <<"-";
    }
}

bool DbManager::contractExists(QString pSymbol)
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT pSymbol FROM contract_ib WHERE pSymbol = (:pSymbol)");
    checkQuery.bindValue(":pSymbol", pSymbol);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "contract exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllContracts()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM contract_ib");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all contracts failed: " << removeQuery.lastError();
    }

    return success;
}

void DbManager::onRetrieveContract()
{
    QMap<QString, ContractInfo> mapContractInfo;
    qDebug() << "retrieve all contracts in db:";
    QSqlQuery query("SELECT * FROM contract_ib");

    while (query.next())
    {
        QDate todayDate = QDate::currentDate();
        QString expiry = query.value("expiry").toString();

        QDate expiryDate = QDate::fromString(expiry, "yyyyMMdd");

        if(expiryDate >= todayDate)
        {

            QString pSymbol = query.value("pSymbol").toString();
            QString contractId = query.value("contractId").toString();
            QString symbol = query.value("symbol").toString();
            QString localSymbol = query.value("localSymbol").toString();
            QString secType = query.value("secType").toString();
            QString exchange = query.value("exchange").toString();
            QString primaryExchange = query.value("primaryExchange").toString();
            QString expiry = query.value("expiry").toString();
            QString currency = query.value("currency").toString();
            QString lotSize = query.value("lotSize").toString();
            QString minTick = query.value("minTick").toString();
            QString multiplier = query.value("multiplier").toString();

            qDebug() << "===" << pSymbol <<"-"
                              << contractId <<"-"
                              << exchange <<"-"
                              << symbol <<"-"
                              << secType <<"-"
                              << expiry <<"-"
                              << lotSize <<"-"
                              << multiplier <<"-";

            ContractInfo contractInfo;
            contractInfo.pSymbol = pSymbol;
            contractInfo.contractId = contractId;
            contractInfo.symbol = symbol;
            contractInfo.localSymbol = localSymbol;
            contractInfo.secType = secType;
            contractInfo.exchange = exchange;
            contractInfo.primaryExchange = primaryExchange;
            contractInfo.expiry = expiry;
            contractInfo.currency = currency;
            contractInfo.lotSize = lotSize;
            contractInfo.minTick = minTick;
            contractInfo.multiplier = multiplier;

            mapContractInfo[pSymbol] =  contractInfo;
        }
    }

    emit ContractRetrieved(mapContractInfo);
}


