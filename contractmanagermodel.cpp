#include "contractmanagermodel.h"
#include <QDebug>
#include <QDateTime>

ContractManagerModel::ContractManagerModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

int ContractManagerModel::rowCount(const QModelIndex & /*parent*/) const
{
   return mKeyList.size() + 1;
}

int ContractManagerModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 15;
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
            case 12:
                return QString("TimeStamp");
            case 13:
                return QString("LotSize");
            case 14:
                return QString("Multiplier");
            }
        }
    }
    return QVariant();
}

bool ContractManagerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{

    int idxRow = index.row();
    int idxCol = index.column();
    int nRow = rowCount();

    QString strValue = value.toString().trimmed();
    QString contractId = this->index(idxRow, 0).data().toString().trimmed();
    QString exchange = this->index(idxRow, 1).data().toString().trimmed();

    if (role == Qt::EditRole && idxCol==0 && idxRow == nRow - 1 )
    {
        if(!mKeyList.contains(strValue) && strValue != "")
        {

            //newline
            mKeyList.append(strValue);

            QStringList tmpStrList;
            tmpStrList<<strValue<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"OFF"<<""<<""<<"";
            mGridData[strValue] = tmpStrList;

            emit dataChanged(createIndex(idxRow,0),createIndex(idxRow,14));
            emit layoutChanged();
            return true;

        }
    }

    if(role == Qt::EditRole && index.column() == 1)
    {
        if(mKeyList.contains(contractId) && strValue != "")
        {

            mGridData[contractId][1] = strValue;
            QModelIndex indexExch = createIndex(idxRow, 1);

            emit dataChanged(indexExch, indexExch);
            return true;

        }

    }


    return true;
}

Qt::ItemFlags ContractManagerModel::flags(const QModelIndex &index) const
{
    int idxRow = index.row();
    int idxCol = index.column();
    int nRow = rowCount();

    QString contractId = this->index(idxRow, 0).data().toString().trimmed();
    QString exchange = this->index(idxRow, 1).data().toString().trimmed();

    if (idxCol == 0 && idxRow == nRow-1) //first column and last empty row
    {

        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    else if(idxCol == 1 && contractId != "" && exchange=="" && idxRow != nRow-1)
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    else
    {
        return QAbstractTableModel::flags(index);
    }
}




void ContractManagerModel::onTickUpdating(const Tick &tick)
{
    QString contractId = tick.contractId;

    if(mGridData.contains(contractId))
    {
        mGridData[contractId][5] = tick.price;
        mGridData[contractId][6] = tick.size;
        mGridData[contractId][12] = QDateTime::fromMSecsSinceEpoch(tick.timeStamp.toLongLong()).toString("hh:mm:ss.zzz");

        int row = mKeyList.indexOf(contractId);
        QModelIndex indexLeft = createIndex(row,5);
        QModelIndex indexRight = createIndex(row,6);
        QModelIndex indexTime = createIndex(row,12);

        emit dataChanged(indexLeft, indexRight);
        emit dataChanged(indexTime, indexTime);


    }

}

void ContractManagerModel::onContractRetrieved(QMap<QString, QStringList> mapContract)
{
    int nContracts = mapContract.size();
    if(nContracts > 0)
    {
        int idx;
        mGridData.clear();
        mKeyList.clear();
        mKeyList = mapContract.keys();

        for(idx = 0; idx < nContracts; idx++)
        {
            QString contractId = mKeyList[idx];
            QStringList tmpStrList;
            tmpStrList << mapContract[contractId][0] << mapContract[contractId][1]    //contractId, exchange
                       << mapContract[contractId][2] << mapContract[contractId][3]    //symbol, secType
                       << mapContract[contractId][4]                                   //expiry,
                       << "" << "" << "" << "" << "" << ""                            //lastPx, lastSz, bidSz, bid,ask, askSize
                       << "OFF" <<""                                                  //status,timeStamp;
                       << mapContract[contractId][5] << mapContract[contractId][6];   //lotSize, multiplier

            mGridData[contractId] = tmpStrList;
        }
        emit layoutChanged();
    }

}

