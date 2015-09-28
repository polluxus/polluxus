#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QWidget>
#include "contractmanagermodel.h"
class QTableView;

class ContractManager : public QWidget
{
    Q_OBJECT
public:
    explicit ContractManager(QWidget *parent = 0);
    virtual ~ContractManager();

    QTableView *pTableView;
    ContractManagerModel *pModel;

    void loadWorkSpace();
    void saveWorkSpace();

signals:

public slots:
};

#endif // CONTRACTMANAGER_H
