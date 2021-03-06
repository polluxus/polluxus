#ifndef CONTRACTMANAGERVIEW_H
#define CONTRACTMANAGERVIEW_H

#include <QWidget>
#include "contractmanagermodel.h"
#include "marketdata.h"


class QTableView;
class QToolBar;
class QPushButton;
class QMenu;
class QAction;
class QSettings;


class ContractManagerView : public QWidget
{
    Q_OBJECT
public:
    explicit ContractManagerView(QWidget *parent = 0);
    virtual ~ContractManagerView();

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

    QString iniFileString;
    QSettings *wsSettings;

signals:
    void ContractClicked(QString symbol, QString bidSz, QString bid, QString ask, QString askSz);

public slots:

    void onCustomMenuRequested(QPoint pos);
    void onAtnSubscribeTriggered();
    void onAtnDeleteTriggered();
    void onClicked(QModelIndex index);

};

#endif // CONTRACTMANAGER_H
