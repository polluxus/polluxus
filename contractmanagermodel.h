#ifndef CONTRACTMANAGERMODEL_H
#define CONTRACTMANAGERMODEL_H

#include <QAbstractTableModel>

class ContractManagerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ContractManagerModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

#endif // CONTRACTMANAGERMODEL_H
