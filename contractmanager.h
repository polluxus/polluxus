#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QObject>
#include <QMap>

#include "instrument.h"

class PosixIBClient;

class ContractManager : public QObject
{
    Q_OBJECT
public:
    explicit ContractManager(QObject *parent = 0);
    virtual ~ContractManager();

    QMap<QString, Instrument*> mapInstrument;
    QMap<QString, ContractInfo> mapContractInfo;
    QMap<QString, QString> mapConIdSymbol;

    QThread *pThread;

    //QReadWriteLock lock;
    ContractInfo getContractInfoBySymbol(QString symbol);

signals:
    void ReqMktData(QString contractId, QString exchange);
    void ReqMktDepth(QString contractId, QString exchange);
    void CancelMktData(QString contractId);
    void CancelMktDepth(QString contractId);

    void InstrumentTraded(const Trade &trade);
    void InstrumentDepthed(const Depth &depth);
    void InstrumentTicked(const Tick &tick);

    void UpdateContractInfo(const ContractInfo &contractInfo);
    void ReqContractInfoErr(QString symbol);

public slots:
    void onContractRetrieved(const QMap<QString, ContractInfo> &mapContractInfo);

    void onAdapterTraded(const Trade &trade);
    void onAdapterDepthed(const Depth &depth);
    void onAdapterTicked(const Tick &tick);

    void onSubscribeMktData(QString symbol);
    void onSubscribeMktDepth(QString symbol);
    void onUnsubscribeMktData(QString symbol);
    void onUnsubscribeMktDepth(QString symbol);

    void onReqContractInfo(QString symbol);
};

#endif // CONTRACTMANAGER_H
