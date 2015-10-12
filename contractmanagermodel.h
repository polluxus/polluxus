#ifndef CONTRACTMANAGERMODEL_H
#define CONTRACTMANAGERMODEL_H

#include <QAbstractTableModel>
#include "marketdata.h"
#include <QMap>

class ContractManagerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ContractManagerModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex & index) const Q_DECL_OVERRIDE ;

    QMap<QString, QStringList> mGridData;
    QList<QString> mKeyList;

signals:
    void ReqContractInfo(QString symbol);

    void SubscribeMktData(QString symbol);
    void SubscribeMktDepth(QString symbol);
    void UnsubscribeMktData(QString symbol);
    void UnsubscribeMktDepth(QString symbol);


public slots:
    void onInstrumentTicked(const Tick &tick);
    void onUpdateContractInfo(const ContractInfo &contractInfo);
    void onReqContractInfoErr(QString symbol);

    void onContractRetrieved(const QMap<QString, ContractInfo> &mapContractInfo);

};

#endif // CONTRACTMANAGERMODEL_H
