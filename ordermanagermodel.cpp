#include "ordermanagermodel.h"

OrderManagerModel::OrderManagerModel(QObject *parent)
  :QAbstractTableModel(parent)
{

}

int OrderManagerModel::rowCount(const QModelIndex & /*parent*/) const
{
   return mGridData.size();
}

int OrderManagerModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 10;
}

QVariant OrderManagerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return mGridData[mKeyList[index.row()]].at(index.column());
    }
    return QVariant();
}

QVariant OrderManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("ORDERID");
            case 1:
                return QString("SYMBOL");
            case 2:
                return QString("B/S");
            case 3:
                return QString("TYPE");
            case 4:
                return QString("TOTAL_QTY");
            case 5:
                return QString("LMT");
            case 6:
                return QString("REMAIN");
            case 7:
                return QString("STATUS");
            case 8:
                return QString("STRATEGY");
            case 9:
                return QString("TIME");
            }
        }
    }
    return QVariant();
}

void OrderManagerModel::onOrderItemUpdated(const OrderItem &orderItem)
{
    long orderId = orderItem.orderId;

    QStringList tmpStrList;
    tmpStrList << QString::number(orderId)
               << orderItem.pSymbol
               << orderItem.action
               << orderItem.orderType
               << QString::number(orderItem.totalQuantity)
               << QString::number(orderItem.lmtPrice)
               << QString::number(orderItem.remainingQuantity)
               << orderItem.orderStatus
               << orderItem.strategyId
               << orderItem.createTimeStamp;
    mGridData[orderId] = tmpStrList;

    if(!mGridData.contains(orderId))
    {
        mKeyList.append(orderId);
        layoutChanged();
    }
    else
    {
        int row = mKeyList.indexOf(orderId);
        QModelIndex indexLeft = createIndex(row,0);
        QModelIndex indexRight = createIndex(row,columnCount()-1);
        emit dataChanged(indexLeft, indexRight);
    }

}
