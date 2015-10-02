#ifndef PolluxusMain_H
#define PolluxusMain_H

#include <QWidget>

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

class PolluxusMain : public QWidget
{
    Q_OBJECT
public:

    PosixIBClient *pIBAdapter;
    MessageProcessor* pMsgProcessor;

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

private:

    int  m_nMouseClick_X_Coordinate;
    int  m_nMouseClick_Y_Coordinate;


    QMenuBar *pMenuBar;
    QToolBar *pToolBar;


    QString host;
    int port;
    int clientId;

    QPushButton *btnNewOrderBookWidget;

    QPushButton *btnTest;
    QPushButton *btnConnect;
    QLabel *lbLight;

    PolluxusLogger *pLogger;
    DigitalClock *pClock;
    ContractManagerView *pContractManagerView;
    ContractManager *pContractManager;

    void createMenuBar();
    void createToolBar();
    void adjustTopBarPosition();


signals:

public slots:

    void onSaveWorkSpaces();

    void onAdapterConnect();
    void onAdapterConnected();
    void onAdapterDisconnected();
    void onNewOrderBookWidget();
    void onTest();
    void onAdjustTimeDiff(qint64 timeDiffMS);
    void onViewContractManager();
    void onViewLogger();

};

#endif // PolluxusMain_H
