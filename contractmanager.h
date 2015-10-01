#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <QWidget>
#include "contractmanagermodel.h"
#include "marketdata.h"


class QTableView;
class QToolBar;
class QPushButton;
class QMenu;
class QAction;

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
    void createContextMenu();

    void loadWorkSpace();
    void saveWorkSpace();

    QMenu *pContextMenu;
    QAction *pAtnSubscribe;
    QAction *pAtnDelete;
    QAction *pAtnDetail;

signals:
    void SubscribeMarketData(QString contractId);
public slots:
    void onTickUpdating(const Tick &tick);
    void onCustomMenuRequested(QPoint pos);
    void onAtnSubscribeTriggered();
};

#endif // CONTRACTMANAGER_H
