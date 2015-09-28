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
    void saveWorkSpace();
    void loadWorkSpace();

private:

    int  m_nMouseClick_X_Coordinate;
    int  m_nMouseClick_Y_Coordinate;


    QMenuBar *pMenuBar;
    QToolBar *pToolBar;

    QLineEdit *editHost;
    QLineEdit *editPort;
    QLineEdit *editClientId;

    QPushButton *btnTest;
    QPushButton *btnConnect;
    QLabel *lbLight;

    PolluxusLogger *pLogger;
    DigitalClock *pClock;
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
    void onTest();
    void onAdjustTimeDiff(qint64 timeDiffMS);


};

#endif // PolluxusMain_H
