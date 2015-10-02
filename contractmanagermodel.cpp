#include "contractmanagermodel.h"

ContractManagerModel::ContractManagerModel(QObject *parent)
    :QAbstractTableModel(parent)
{

    QStringList tmpStrList1;
    tmpStrList1<<"167205842"<<"GLOBEX"<<"ES"<<"FUT"<<"201512"<<"1980.5"<<"2"<<"120"<<"1960.5"<<"1990.25"<<"110"<<"OFF";

    QStringList tmpStrList2;
    tmpStrList2<<"167205847"<<"GLOBEX"<<"NQ"<<"FUT"<<"201512"<<"123.5"<<"6"<<"28"<<"120.5"<<"125.0"<<"90"<<"OFF";

    mGridData["167205842"] = tmpStrList1;
    mGridData["167205847"] = tmpStrList2;

    mKeyList.append("167205842");
    mKeyList.append("167205847");

}

int ContractManagerModel::rowCount(const QModelIndex & /*parent*/) const
{
   return mKeyList.size() + 1;
}

int ContractManagerModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 12;
}

QVariant ContractManagerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
       if(index.row() == mKeyList.size())
       {
           return QVariant ();
       }
       else
       {
           return mGridData[mKeyList[index.row()]].at(index.column());
       }


    }
    return QVariant();
}

QVariant ContractManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("ContractId");
            case 1:
                return QString("Exchange");
            case 2:
                return QString("Symbol");
            case 3:
                return QString("SecType");
            case 4:
                return QString("Expiry");
            case 5:
                return QString("LastPx");
            case 6:
                return QString("Qty");
            case 7:
                return QString("BidSz");
            case 8:
                return QString("Bid");
            case 9:
                return QString("Ask");
            case 10:
                return QString("AskSz");
            case 11:
                return QString("Status");

            }
        }
    }
    return QVariant();
}

bool ContractManagerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole && (index.column()==0 || index.column()==1) )
    {
        //save value from editor to member m_gridData
        if(value.toString().trimmed() != "")   mGridData[mKeyList[index.row()]][index.column()]= value.toString();
    }

    return true;
}

Qt::ItemFlags ContractManagerModel::flags(const QModelIndex &index) const
{
    if (index.column()==0 || index.column()==1)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    else
        return QAbstractTableModel::flags(index);
}

void ContractManagerModel::onTickUpdating(const Tick &tick)
{
    QString contractId = tick.contractId;

    if(mGridData.contains(contractId))
    {
        mGridData[contractId][5] = tick.price;
        mGridData[contractId][6] = tick.size;

        int row = mKeyList.indexOf(contractId);
        QModelIndex indexLeft = createIndex(row,5);
        QModelIndex indexRight = createIndex(row,6);

        emit dataChanged(indexLeft, indexRight);


    }

}

