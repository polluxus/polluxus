#ifndef ORDERMANAGERMODEL_H
#define ORDERMANAGERMODEL_H

#include <QAbstractTableModel>
#include "marketdata.h"
#include <QMap>

class OrderManagerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OrderManagerModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;


    QMap<long, QStringList> mGridData;
    QList<long> mKeyList;
signals:

public slots:
    void onOrderItemUpdated(const OrderItem &orderItem);
};

#endif // ORDERMANAGERMODEL_H
