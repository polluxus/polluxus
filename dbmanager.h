#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QStringList>
class QThread;



class DbManager  : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(const QString& path, QObject *parent = 0);
    virtual ~DbManager();

    bool isOpen() const;
    bool addContract(QString contractId,  QString exchange, QString symbol,QString secType,
                     QString expiery, QString lotSize, QString multiplier);
    bool removeContract(QString contractId);
    bool contractExists(QString contractId);
    void printAllContracts();
    bool removeAllContracts();

    QThread *pThread;

signals:
    void ContractRetrieved(QMap<QString, QStringList> mapContract);

public slots:
    void onRetrieveContract();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H



