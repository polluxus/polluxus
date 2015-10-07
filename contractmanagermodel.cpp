#include "contractmanagermodel.h"
#include <QDebug>
#include <QDateTime>
#include "contractmanager.h"

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
    return 14;
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
                return QString("SYMBOL");
            case 1:
                return QString("EXPIRY");
            case 2:
                return QString("BID_SZ");
            case 3:
                return QString("BID");
            case 4:
                return QString("ASK");
            case 5:
                return QString("ASK_SZ");
            case 6:
                return QString("LAST");
            case 7:
                return QString("LAST_SZ");
            case 8:
                return QString("HIGH");
            case 9:
                return QString("LOW");
            case 10:
                return QString("VOLUME");
            case 11:
                return QString("CLOSE");
            case 12:
                return QString("%CHG");
            case 13:
                return QString("TIME");

            }
        }
    }
    return QVariant();
}

bool ContractManagerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{

    QString strValue = value.toString().trimmed();

    if (role == Qt::EditRole && index.column()==0 )
    {
        if(!mKeyList.contains(strValue) && strValue != "")
        {
            //newline
            mKeyList.append(strValue);

            QStringList tmpStrList;
            tmpStrList<<strValue;
            for(int i = 1; i < columnCount()-1; i++) tmpStrList << "";

            mGridData[strValue] = tmpStrList;

            emit dataChanged(createIndex(index.row(),0),createIndex(index.row(),columnCount()-1));
            emit layoutChanged();
            emit ReqContractInfo(strValue);
            return true;
        }
    }

    return true;
}

Qt::ItemFlags ContractManagerModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0 && index.row() == columnCount()-1) //first column, last row editable
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }
    else
    {
        return QAbstractTableModel::flags(index);
    }

}


void ContractManagerModel::onInstrumentTicked(Tick tick)
{
    QString symbol = tick.contractId;

    if(mGridData.contains(symbol))
    {
        int tickType = tick.typeId.toInt();
        int offset = 2;

        mGridData[symbol][offset + tickType] = tick.value;
        mGridData[symbol][13] = QDateTime::fromMSecsSinceEpoch(tick.timeStamp.toLongLong()).toString("hh:mm:ss.zzz");

        int row = mKeyList.indexOf(symbol);
        QModelIndex indexLeft = createIndex(row,0);
        QModelIndex indexRight = createIndex(row,columnCount()-1);
        emit dataChanged(indexLeft, indexRight);

    }

}


void ContractManagerModel::onUpdateContractInfo(ContractInfo contractInfo)
{
    QString symbol = contractInfo.pSymbol;
    int idxRow = mKeyList.indexOf(symbol);
    QStringList tmpStrlist;
    tmpStrlist<<symbol
             <<contractInfo.expiry
             <<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"";
    mGridData[symbol] = tmpStrlist;
    QModelIndex indexLeft = createIndex(idxRow,0);
    QModelIndex indexRight = createIndex(idxRow,columnCount()-1);
    emit dataChanged(indexLeft, indexRight);

}

void ContractManagerModel::onReqContractInfoErr(QString symbol)
{
    int idxRow = mKeyList.indexOf(symbol);
    mGridData.remove(symbol);
    mKeyList.removeAll(symbol);
    removeRow(idxRow);
    emit layoutChanged();
}
