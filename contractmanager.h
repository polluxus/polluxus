#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QWidget>
#include "contractmanagermodel.h"
#include "marketdata.h"


class QTableView;
class QToolBar;
class QPushButton;

class ContractManager : public QWidget
{
    Q_OBJECT
public:
    explicit ContractManager(QWidget *parent = 0);
    virtual ~ContractManager();

    QToolBar *pToolBar;
    QPushButton *btnSubscribe;

    QTableView *pTableView;
    ContractManagerModel *pModel;

    void createToolBar();

    void loadWorkSpace();
    void saveWorkSpace();

signals:

public slots:
    void onTickUpdating(const Tick &tick);
};

#endif // CONTRACTMANAGER_H
