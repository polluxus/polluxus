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

signals:
    void ReqMktData(QString contractId, QString exchange);
    void ReqMktDepth(QString contractId, QString exchange);
    void CancelMktData(QString contractId);
    void CancelMktDepth(QString contractId);

    void InstrumentTraded(Trade trade);
    void InstrumentDepthed(Depth depth);
    void InstrumentTicked(Tick tick);

    void UpdateContractInfo(ContractInfo contractInfo);
    void ReqContractInfoErr(QString symbol);

public slots:
    void onContractRetrieved(QMap<QString, ContractInfo> mapContractInfo);

    void onAdapterTraded(Trade trade);
    void onAdapterDepthed(Depth depth);
    void onAdapterTicked(Tick tick);

    void onSubscribeMktData(QString symbol);
    void onSubscribeMktDepth(QString symbol);
    void onUnSubscribeMktData(QString symbol);
    void onUnSubscribeMktDepth(QString symbol);

    void onReqContractInfo(QString symbol);
};

#endif // CONTRACTMANAGER_H
