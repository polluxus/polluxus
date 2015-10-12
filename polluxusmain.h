#ifndef PolluxusMain_H
#define PolluxusMain_H

#include <QWidget>
#include "testsignalslot.h"

class PosixIBClient;
class MessageProcessor;
class QMouseEvent;
class QMenuBar;
class QToolBar;
class QLineEdit;
class QPushButton;
class QLabel;
class PolluxusLogger;
class DigitalClock;
class ContractManager;
class ContractManagerView;
class QSettings;
class DbManager;
class QHBoxLayout;


class PolluxusMain : public QWidget
{
    Q_OBJECT
public:

    PosixIBClient *pDataIBAdapter;
    MessageProcessor *pDataMsgProcessor;
    PosixIBClient *pOrderIBAdapter;
    MessageProcessor *pOrderMsgProcessor;

    explicit PolluxusMain(QWidget *parent = 0);
    virtual ~PolluxusMain();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


public:

    QString iniFileString;
    QSettings *wsSettings;

    void saveWorkSpace();
    void loadWorkSpace();
    void loadGateway();
    void loadDbPath();

private:

    int  m_nMouseClick_X_Coordinate;
    int  m_nMouseClick_Y_Coordinate;

    QLabel *pLogo;
    QMenuBar *pMenuBar;
    QToolBar *pToolBar;
    QHBoxLayout *hLayout;

    QString host;
    int port;
    int clientId;

    QPushButton *btnNewOrderBookWidget;
    QPushButton *btnTest;
    QPushButton *btnConnectData;
    QPushButton *btnConnectOrder;

    QLabel *lbLight;

    PolluxusLogger *pLogger;
    DigitalClock *pClock;
    ContractManagerView *pContractManagerView;
    ContractManager *pContractManager;
    DbManager *pDbManager;
    QString dbPath;

    void createAppLogo();
    void createMenuBar();
    void createToolBar();
    void createLayout();

    void connectSignalSlot();
    void adjustTopBarPosition();

    TestSignalSlot *pTestSignalSlot;
signals:

public slots:

    void onSaveWorkSpaces();

    void onAdapterConnectData();
    void onAdapterConnectOrder();
    void onAdapterConnected(int connType);
    void onAdapterDisconnected(int connType);
    void onNewOrderBookWidget();
    void onTest();
    void onAdjustTimeDiff(qint64 timeDiffMS);
    void onViewContractManager();
    void onViewLogger();

};

#endif // PolluxusMain_H
