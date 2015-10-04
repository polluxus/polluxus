#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QThread>



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

bool DbManager::addContract(QString contractId, QString exchange, QString symbol,  QString secType,
                            QString expiry, QString lotSize, QString multiplier)
{
    bool success = false;

    if (!contractId.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO contract (contractId, exchange, symbol, secType, expiry, lotSize, multiplier) "
                         "VALUES (:contractId, :exchange, :symbol, :secType, :expiry, :lotSize, :multiplier)");
        queryAdd.bindValue(":contractId", contractId);
        queryAdd.bindValue(":exchange", exchange);
        queryAdd.bindValue(":symbol", symbol);
        queryAdd.bindValue(":secType", secType);
        queryAdd.bindValue(":expiry", expiry);
        queryAdd.bindValue(":lotSize", lotSize);
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

bool DbManager::removeContract(QString contractId)
{
    bool success = false;

    if (contractExists(contractId))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM contract WHERE contractId = (:contractId)");
        queryDelete.bindValue(":contractId", contractId);
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
    QSqlQuery query("SELECT * FROM contract");

    while (query.next())
    {
        QString contractId = query.value(0).toString();
        QString exchange = query.value(1).toString();
        QString symbol = query.value(2).toString();
        QString secType = query.value(3).toString();
        QString expiry = query.value(4).toString();
        QString lotSize = query.value(5).toString();
        QString multiplier = query.value(6).toString();

        qDebug() << "===" << contractId <<"-"
                          << exchange <<"-"
                          << symbol <<"-"
                          << secType <<"-"
                          << expiry <<"-"
                          << lotSize <<"-"
                          << multiplier <<"-";
    }
}

bool DbManager::contractExists(QString contractId)
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT contractId FROM contract WHERE contractId = (:contractId)");
    checkQuery.bindValue(":contractId", contractId);

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
    removeQuery.prepare("DELETE FROM contract");

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
    QMap<QString, QStringList> mapContract;
    qDebug() << "retrieve all contracts in db:";
    QSqlQuery query("SELECT * FROM contract");

    while (query.next())
    {
        QString contractId = query.value(0).toString();
        QString exchange = query.value(1).toString();
        QString symbol = query.value(2).toString();
        QString secType = query.value(3).toString();
        QString expiry = query.value(4).toString();
        QString lotSize = query.value(5).toString();
        QString multiplier = query.value(6).toString();

        qDebug() << "===" << contractId <<"-"
                          << exchange <<"-"
                          << symbol <<"-"
                          << secType <<"-"
                          << expiry <<"-"
                          << lotSize <<"-"
                          << multiplier <<"-";

        QStringList tmpStrList;
        tmpStrList << contractId << exchange << symbol << secType << expiry << lotSize << multiplier;
        mapContract[contractId] =  tmpStrList;
    }

    emit ContractRetrieved(mapContract);
}


