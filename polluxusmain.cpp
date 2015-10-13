#include "polluxusmain.h"
#include <QMouseEvent>

#include <QDesktopWidget>
#include <QDebug>
#include "posixibclient.h"
#include "messageprocessor.h"
#include "polluxuslogger.h"
#include "digitalclock.h"
#include "marketdata.h"
#include "contractmanager.h"
#include "contractmanagerview.h"
#include "ordermanager.h"
#include "ordermanagerview.h"

#include "orderbookwidget.h"
#include "dbmanager.h"
#include "polluxusutility.h"
#include "instrument.h"
#include <QHBoxLayout>

PolluxusMain::PolluxusMain(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{

    m_nMouseClick_X_Coordinate = 0;
    m_nMouseClick_Y_Coordinate = 0;

    iniFileString = QDir::currentPath() + "/workspace.ini";
    wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    loadDbPath();
    loadGateway();

    createAppLogo();
    createMenuBar();
    createToolBar();
    createLayout();

    adjustTopBarPosition();

    pDataIBAdapter = new PosixIBClient();
    pDataMsgProcessor = new MessageProcessor(pDataIBAdapter);
    pOrderIBAdapter = new PosixIBClient();
    pOrderMsgProcessor = new MessageProcessor(pOrderIBAdapter);

    pLogger = new PolluxusLogger(this);
    pLogger->show();

    pDbManager = new DbManager(dbPath);

    pContractManager = new ContractManager();
    pContractManagerView = new ContractManagerView(this);
    pContractManagerView->show();

    pOrderManager = new OrderManager(pContractManager);
    pOrderManagerView = new OrderManagerView(this);
    pOrderManagerView->show();

    pTradePad = new TradePadWidget(this);
    pTradePad->show();

    loadWorkSpace();

    PolluxusUtility::registerMetaType();

    pTestSignalSlot = new TestSignalSlot;



    connectSignalSlot();
    QMetaObject::invokeMethod(pDbManager, "onRetrieveContract", Qt::QueuedConnection);

}

PolluxusMain::~PolluxusMain()
{

    saveWorkSpace();
    if(!pDataIBAdapter) delete pDataIBAdapter;
    if(!wsSettings) delete wsSettings;
    if(!pContractManager) delete pContractManager;
    if(!pContractManagerView) delete pContractManagerView;
    if(!pLogger) delete pLogger;
    if(!pDataMsgProcessor) delete pDataMsgProcessor;
    if(!pDbManager) delete pDbManager;
}


void PolluxusMain::connectSignalSlot()
{
    //connect signal slot here

    connect(btnNewOrderBookWidget, SIGNAL(clicked(bool)), this, SLOT(onNewOrderBookWidget()));
    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(onTest()), Qt::DirectConnection);
    connect(btnConnectData, SIGNAL(toggled(bool)), this, SLOT(onAdapterConnectData()));
    connect(btnConnectOrder, SIGNAL(toggled(bool)), this, SLOT(onAdapterConnectOrder()));

    connect(pOrderIBAdapter, SIGNAL(AdapterConnected(int)), this, SLOT(onAdapterConnected(int)));
    connect(pOrderIBAdapter, SIGNAL(AdapterDisconnected(int)), this, SLOT(onAdapterDisconnected(int)));

    connect(pDataIBAdapter, SIGNAL(AdapterConnected(int)), this, SLOT(onAdapterConnected(int)));
    connect(pDataIBAdapter, SIGNAL(AdapterDisconnected(int)), this, SLOT(onAdapterDisconnected(int)));
    connect(pDataIBAdapter, SIGNAL(AdjustTimeDiff(qint64)), this, SLOT(onAdjustTimeDiff(qint64)));
    connect(pDataIBAdapter, SIGNAL(AdapterTraded(const Trade&)), pContractManager, SLOT(onAdapterTraded(const Trade&)));
    connect(pDataIBAdapter, SIGNAL(AdapterDepthed(const Depth&)), pContractManager, SLOT(onAdapterDepthed(const Depth&)));
    connect(pDataIBAdapter, SIGNAL(AdapterTicked(const Tick&)), pContractManager, SLOT(onAdapterTicked(const Tick&)), Qt::DirectConnection);
    //connect(pDataIBAdapter, SIGNAL(AdapterTraded(const Trade&)), pContractManager, SLOT(onAdapterTraded(const Trade&)), Qt::DirectConnection);
    //connect(pDataIBAdapter, SIGNAL(AdapterDepthed(const Depth&)), pContractManager, SLOT(onAdapterDepthed(const Depth&)), Qt::DirectConnection);
    //connect(pDataIBAdapter, SIGNAL(AdapterTicked(const Tick&)), pContractManager, SLOT(onAdapterTicked(const Tick&)), Qt::DirectConnection);

    //connect(pDataIBAdapter, SIGNAL(DummySignalTraded(const QString&)), pTestSignalSlot, SLOT(onDummySignalTraded(const QString&)), Qt::QueuedConnection);
    //connect(pDataIBAdapter, SIGNAL(DummySignalTicked(const QString&)), pTestSignalSlot, SLOT(onDummySignalTicked(const QString&)), Qt::QueuedConnection);

    connect(pOrderIBAdapter, SIGNAL(OrderUpdated(const OrderUpdate&)), pOrderManager, SLOT(onOrderUpdated(const OrderUpdate&)));

    connect(pDbManager, SIGNAL(ContractRetrieved(const QMap<QString,ContractInfo>)), pContractManager, SLOT(onContractRetrieved(const QMap<QString,ContractInfo>)));
    connect(pDbManager, SIGNAL(ContractRetrieved(const QMap<QString,ContractInfo>)), pContractManagerView->pModel, SLOT(onContractRetrieved(const QMap<QString,ContractInfo>)));

    connect(pContractManager, SIGNAL(ReqMktData(QString,QString)), pDataIBAdapter, SLOT(onReqMktData(QString,QString)));
    connect(pContractManager, SIGNAL(CancelMktData(QString)), pDataIBAdapter, SLOT(onCancelMktData(QString)));
    connect(pContractManager, SIGNAL(ReqMktDepth(QString,QString)), pDataIBAdapter, SLOT(onReqMktDepth(QString,QString)));
    connect(pContractManager, SIGNAL(CancelMktDepth(QString)), pDataIBAdapter, SLOT(onCancelMktDepth(QString)));
    connect(pContractManager, SIGNAL(InstrumentTicked(Tick)), pContractManagerView->pModel, SLOT(onInstrumentTicked(Tick)));
    connect(pContractManager, SIGNAL(UpdateContractInfo(const ContractInfo)), pContractManagerView->pModel, SLOT(onUpdateContractInfo(const ContractInfo)));
    connect(pContractManager, SIGNAL(ReqContractInfoErr(QString)), pContractManagerView->pModel, SLOT(onReqContractInfoErr(QString)));
    connect(pContractManager, SIGNAL(InstrumentTicked(Tick)), pTradePad, SLOT(onInstrumentTicked(Tick)));

    connect(pContractManagerView, SIGNAL(ContractClicked(QString,QString,QString,QString,QString)), pTradePad, SLOT(onContractClicked(QString,QString,QString,QString,QString)));

    connect(pContractManagerView->pModel, SIGNAL(SubscribeMktData(QString)),   pContractManager, SLOT(onSubscribeMktData(QString)));
    connect(pContractManagerView->pModel, SIGNAL(SubscribeMktDepth(QString)), pContractManager, SLOT(onSubscribeMktDepth(QString)));
    connect(pContractManagerView->pModel, SIGNAL(UnsubscribeMktData(QString)),   pContractManager, SLOT(onUnsubscribeMktData(QString)));
    connect(pContractManagerView->pModel, SIGNAL(UnsubscribeMktDepth(QString)),  pContractManager, SLOT(onUnsubscribeMktDepth(QString)));
    connect(pContractManagerView->pModel, SIGNAL(ReqContractInfo(QString)),   pContractManager, SLOT(onReqContractInfo(QString)));

    connect(pOrderManager, SIGNAL(NewOrder(const OrderItem&)), pOrderIBAdapter, SLOT(onNewOrder(const OrderItem&)));
    connect(pOrderManager, SIGNAL(OrderItemUpdated(const OrderItem&)), pOrderManagerView->pModel, SLOT(onOrderItemUpdated(const OrderItem&)));

    connect(pTradePad, SIGNAL(OrderSubmit(QString,QString,QString,double,int,QString)), pOrderManager, SLOT(onOrderSubmit(QString,QString,QString,double,int,QString)));
}

void PolluxusMain::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << "mousePressEvent()";
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void PolluxusMain::mouseMoveEvent(QMouseEvent* event)
{
    //qDebug() << "mouseMoveEvent()";
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void PolluxusMain::mouseReleaseEvent(QMouseEvent* event)
{
    //qDebug() << "mouseReleaseEvent()";
    if(event->globalY() < 100)
    {
        //move(-(event->globalX()),-(event->globalY()));
        //move(-50, -50);

        qDebug()<<this->rect().topLeft();
    }
}

void PolluxusMain::createAppLogo()
{
    pLogo = new QLabel();
    pLogo->setFixedWidth(20);
    pLogo->setFixedHeight(20);
    pLogo->setScaledContents( true );
    pLogo->setPixmap(QPixmap(":/images/setup.png"));
}

void PolluxusMain::createMenuBar()
{
    pMenuBar = new QMenuBar;

    QAction *quit = new QAction("&Quit", this);
    QAction *saveWS = new QAction("&Save Workspace", this);

//    QMenu *logo;
//    logo = pMenuBar->addMenu(QIcon(":/images/setup.png"), "Polluxus");

    QMenu *file;
    file = pMenuBar->addMenu("&File");
    file->addAction(quit);
    file->addAction(saveWS);

    QAction *viewCM = new QAction("Show ContractManager", this);
    QAction *viewLogger = new QAction("Show Logger", this);

    QMenu *view;
    view = pMenuBar->addMenu("&View");
    view->addAction(viewCM);
    view->addAction(viewLogger);


    QMenu *trading;
    trading = pMenuBar->addMenu("&Trading");
    trading->addSeparator();


    QMenu *about;
    about = pMenuBar->addMenu("&About");
    about->addSeparator();


    pMenuBar->addSeparator();

    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(saveWS, SIGNAL(triggered()), this, SLOT(onSaveWorkSpaces()));
    connect(viewCM,SIGNAL(triggered()), this, SLOT(onViewContractManager()));
    connect(viewLogger,SIGNAL(triggered()), this, SLOT(onViewLogger()));

    pMenuBar->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

void PolluxusMain::createToolBar()
{

    pToolBar = new QToolBar;


    btnNewOrderBookWidget = new QPushButton("New OrderBook");
    btnTest = new QPushButton(tr("test"));
    btnConnectData = new QPushButton(tr("DataAPI"));
    btnConnectData->setCheckable(true);
    btnConnectData->setIcon(QIcon(":/images/bullet-red.png"));

    btnConnectOrder = new QPushButton(tr("OrderAPI"));
    btnConnectOrder->setCheckable(true);
    btnConnectOrder->setIcon(QIcon(":/images/bullet-red.png"));

    pClock = new DigitalClock(this);

    lbLight = new QLabel();
    lbLight->setFixedWidth(24);
    lbLight->setFixedHeight(24);
    lbLight->setScaledContents( true );
    lbLight->setAlignment(Qt::AlignRight);
    lbLight->setPixmap(QPixmap(":/images/bullet-red.png"));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    pToolBar->addWidget(spacer);

    pToolBar->addWidget(btnNewOrderBookWidget);
    pToolBar->addWidget(btnTest);
    pToolBar->addWidget(btnConnectData);
    pToolBar->addWidget(btnConnectOrder);

    pToolBar->addWidget(pClock);
    pToolBar->addWidget(lbLight);

    //pClock->show();

    pToolBar->adjustSize();

    pToolBar->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );


}

void PolluxusMain::createLayout()
{
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setMargin(0);
    hLayout->setContentsMargins(0,0,0,0);

    hLayout->addWidget(pLogo);
    hLayout->addWidget(pMenuBar);
    hLayout->addWidget(spacer);
    hLayout->addWidget(pToolBar);
    setLayout(hLayout);
}

void PolluxusMain::adjustTopBarPosition()
{
    resize(QDesktopWidget().availableGeometry().width(), 24);
    move(0, 0);
    this->setFixedSize(this->size());
}



void PolluxusMain::onTest()
{
    QMetaObject::invokeMethod(pDataIBAdapter, "onReqAccountUpdates", Qt::QueuedConnection);

//    if (pDbManager->isOpen())
//    {

//        pDbManager->printAllContracts();

//        db.addContract("167205842", "GLOBEX", "ES", "FUT" ,"201512", "1", "50.0");

//        //db.printAllContracts();

//    }
//    else
//    {
//        qDebug() << "Database is not open!";
//    }

}

void PolluxusMain::onAdjustTimeDiff(qint64 timeDiffMS)
{
    pClock->setTimeDiffMS(timeDiffMS);
    qDebug() << "Signal AdjustTimeDiff() received";
}

void PolluxusMain::onAdapterConnectData()
{
    if(btnConnectData->isChecked())
    {
        qDebug() << "MainWindow:Hi I am connecting IB Data API.------"  << QThread::currentThreadId();
        QMetaObject::invokeMethod(pDataIBAdapter, "onConnect", Qt::QueuedConnection,
                                  Q_ARG(QString, host),
                                  Q_ARG(int, port),
                                  Q_ARG(int, clientId),
                                  Q_ARG(int, 0));               // 0 - Data API

        btnConnectData->setEnabled(false);
        btnConnectData->setIcon(QIcon(":/images/wait.png"));


        lbLight->setPixmap(QPixmap(":/images/wait.png"));

    }

    else
    {
        qDebug() << "MainWindow:Hi I am disconnecting IB Data API.------";

        QMetaObject::invokeMethod(pDataIBAdapter, "onDisconnect", Qt::QueuedConnection );

        btnConnectData->setEnabled(false);
        btnConnectData->setIcon(QIcon(":/images/bullet-grey.png"));

        lbLight->setPixmap(QPixmap(":/images/bullet-grey.png"));

    }
}

void PolluxusMain::onAdapterConnectOrder()
{
    if(btnConnectOrder->isChecked())
    {
        qDebug() << "MainWindow:Hi I am connecting IB Order API.------"  << QThread::currentThreadId();
        QMetaObject::invokeMethod(pOrderIBAdapter, "onConnect", Qt::QueuedConnection,
                                  Q_ARG(QString, host),
                                  Q_ARG(int, port),
                                  Q_ARG(int, clientId + 1),
                                  Q_ARG(int , 1));                                  //1 - Order API

        btnConnectOrder->setEnabled(false);
        btnConnectOrder->setIcon(QIcon(":/images/wait.png"));


        lbLight->setPixmap(QPixmap(":/images/wait.png"));

    }

    else
    {
        qDebug() << "MainWindow:Hi I am disconnecting IB Order API.------";
        QMetaObject::invokeMethod(pOrderIBAdapter, "onDisconnect", Qt::QueuedConnection );

        btnConnectOrder->setEnabled(false);
        btnConnectOrder->setIcon(QIcon(":/images/bullet-grey.png"));

        lbLight->setPixmap(QPixmap(":/images/bullet-grey.png"));

    }
}


void PolluxusMain::onAdapterConnected(int connType)
{
    qDebug() << "MainWindow:Recv connected signal from Posix.------"<< connType;

    if(connType == 0)   //Data API
    {
        btnConnectData->setEnabled(true);
        btnConnectData->setIcon(QIcon(":/images/bullet-green.png"));
        lbLight->setPixmap(QPixmap(":/images/bullet-green.png"));
        pDataMsgProcessor->start();
    }
    else
    {
        btnConnectOrder->setEnabled(true);
        btnConnectOrder->setIcon(QIcon(":/images/bullet-green.png"));
        lbLight->setPixmap(QPixmap(":/images/bullet-green.png"));
        pOrderMsgProcessor->start();
    }

    //QMetaObject::invokeMethod(pDataIBAdapter, "onReqCurrentTime", Qt::QueuedConnection);
}

void PolluxusMain::onAdapterDisconnected(int connType)
{
    qDebug() << "MainWindow:Recv disconnected signal from Posix.------" << connType;

    if(connType == 0)   //Data API
    {
    btnConnectData->setEnabled(true);
    btnConnectData->setIcon(QIcon(":/images/bullet-red.png"));
    lbLight->setPixmap(QPixmap(":/images/bullet-red.png"));
    }
    else
    {
        btnConnectOrder->setEnabled(true);
        btnConnectOrder->setIcon(QIcon(":/images/bullet-red.png"));
        lbLight->setPixmap(QPixmap(":/images/bullet-red.png"));
    }
}


void PolluxusMain::saveWorkSpace()
{
    //qDebug() << "PolluxusMain::saveWorkSpace";

    wsSettings->setValue("appname", "Polluxus");

    wsSettings->beginGroup("PolluxusMain");
    wsSettings->setValue("geometry", saveGeometry());
    wsSettings->setValue( "maximized", isMaximized());
    if ( !isMaximized() ) {
            wsSettings->setValue( "pos", pos() );
            wsSettings->setValue( "size", size() );
        }
    wsSettings->endGroup();
    wsSettings->sync();
}

void PolluxusMain::loadGateway()
{
    //qDebug() << "PolluxusMain::loadGateway";

    wsSettings->beginGroup("Gateway");
    host = wsSettings->value( "host", "127.0.0.1").toString();
    port = wsSettings->value( "port", 4001).toInt();
    clientId = wsSettings->value( "clientId", 1).toInt();
    wsSettings->endGroup();
}

void PolluxusMain::loadDbPath()
{
    qDebug() << "PolluxusMain::loadDbPath";

    wsSettings->beginGroup("Database");
    dbPath = wsSettings->value("dbpath", "").toString();
    wsSettings->endGroup();
}

void PolluxusMain::loadWorkSpace()
{
    //qDebug() << "PolluxusMain::loadWorkSpace";

    wsSettings->beginGroup("PolluxusMain");
    restoreGeometry(wsSettings->value( "geometry", saveGeometry() ).toByteArray());
    move(wsSettings->value( "pos", pos() ).toPoint());
    resize(wsSettings->value( "size", size()).toSize());
    if ( wsSettings->value( "maximized", isMaximized() ).toBool() )
    {
        showMaximized();
    }
    wsSettings->endGroup();
}

void PolluxusMain::onSaveWorkSpaces()
{
    this->saveWorkSpace();
    pLogger->saveWorkSpace();
    pContractManagerView->saveWorkSpace();
}

void PolluxusMain::onNewOrderBookWidget()
{

//    qDebug() << "onNewOrderBookWidget()";
//    OrderBookWidget *obWidget = new OrderBookWidget(this);
//    obWidget->show();
}

void PolluxusMain::onViewContractManager()
{
    if(!pContractManagerView)
    {
        pContractManagerView = new ContractManagerView(this);
        connect(pDataIBAdapter, SIGNAL(TickUpdating(const Tick)), pContractManagerView, SLOT(onTickUpdating(const Tick)));
        pContractManagerView->show();
    }
    else
    {
        if (!pContractManagerView->isVisible()) pContractManagerView->show();
    }
}

void PolluxusMain::onViewLogger()
{
    if(!pLogger)
    {
        pLogger = new PolluxusLogger(this);
        connect(pDataIBAdapter, SIGNAL(OrderUpdated(QString)), pLogger, SLOT(onOrderUpdated(QString)));
        pLogger->show();
    }
    else
    {
        if (!pLogger->isVisible()) pLogger->show();
    }
}
