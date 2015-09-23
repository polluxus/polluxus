#include "polluxusmain.h"
#include <QMouseEvent>

#include <QDesktopWidget>
#include <QDebug>
#include "posixibclient.h"
#include "messageprocessor.h"
#include "polluxuslogger.h"
#include "digitalclock.h"
#include "marketdata.h"



PolluxusMain::PolluxusMain(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{

    m_nMouseClick_X_Coordinate = 0;
    m_nMouseClick_Y_Coordinate = 0;

    createMenuBar();
    createToolBar();


    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setMargin(0);
    hLayout->setContentsMargins(0,0,0,0);


    hLayout->addWidget(pMenuBar);
    hLayout->addWidget(pToolBar);
    setLayout(hLayout);

    adjustTopBarPosition();


    pIBAdapter = new PosixIBClient;

    std::shared_ptr<PosixIBClient> pIBClient(pIBAdapter);
    pMsgProcessor = new MessageProcessor(pIBClient);

    connect(pIBAdapter, SIGNAL(AdapterConnected()), this, SLOT(onAdapterConnected()));
    connect(pIBAdapter, SIGNAL(AdapterDisconnected()), this, SLOT(onAdapterDisconnected()));


    pLogger = new PolluxusLogger(this);
    pLogger->show();

    connect(pIBAdapter, SIGNAL(OrderUpdated(QString)), pLogger, SLOT(onOrderUpdated(QString)));
    loadWorkSpace();

    qRegisterMetaType<Tick>("Tick");
    qRegisterMetaType<Tick>("Tick&");
    qRegisterMetaType<Depth>("Depth");
    qRegisterMetaType<Depth>("Depth&");
    qRegisterMetaType<TickerData>("TickerData");
    qRegisterMetaType<TickerData>("TickerData&");
}

PolluxusMain::~PolluxusMain()
{

    saveWorkSpace();
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
        move(-(event->globalX()),-(event->globalY()));
    }
}

void PolluxusMain::createMenuBar()
{
    pMenuBar = new QMenuBar;

    QAction *quit = new QAction("&Quit", this);
    QAction *saveWS = new QAction("&Save Workspace", this);

    QMenu *file;
    file = pMenuBar->addMenu(QIcon(":/images/setup.png"), "Polluxus");
    file->addAction(quit);
    file->addAction(saveWS);

    pMenuBar->addSeparator();

    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(saveWS, SIGNAL(triggered()), this, SLOT(onSaveWorkSpaces()));

    pMenuBar->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

void PolluxusMain::createToolBar()
{

    pToolBar = new QToolBar;
    //const char *host, int port,  int clientID
    QLabel *lblHost = new QLabel("Host:");
    QLabel *lblPort = new QLabel("Port:");
    QLabel *lblClientId = new QLabel("ClientId:");

    editHost = new QLineEdit();
    editPort = new QLineEdit();
    editClientId = new QLineEdit();

    editHost->setText("127.0.0.1");
    editPort->setText("4001");
    editClientId->setText("1");

    btnTest = new QPushButton(tr("test"));

    btnConnect = new QPushButton(tr("Connect"));
    btnConnect->setCheckable(true);


    DigitalClock *pClock = new DigitalClock(this);



    lbLight = new QLabel();
    lbLight->setFixedWidth(24);
    lbLight->setFixedHeight(24);
    lbLight->setScaledContents( true );
    lbLight->setAlignment(Qt::AlignRight);
    lbLight->setPixmap(QPixmap(":/images/bullet-red.png"));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    pToolBar->addWidget(lblHost);
     pToolBar->addWidget(editHost);

    pToolBar->addWidget(lblPort);
    pToolBar->addWidget(editPort);

    pToolBar->addWidget(lblClientId);
    pToolBar->addWidget(editClientId);

    pToolBar->addWidget(btnTest);
    pToolBar->addWidget(btnConnect);
    pToolBar->addWidget(spacer);
    pToolBar->addWidget(pClock);
    pToolBar->addWidget(lbLight);

    //pClock->show();

    pToolBar->adjustSize();

    pToolBar->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(onTest()), Qt::DirectConnection);
    connect(btnConnect, SIGNAL(toggled(bool)), this, SLOT(onAdapterConnect()), Qt::DirectConnection);

}

void PolluxusMain::adjustTopBarPosition()
{
    resize(QDesktopWidget().availableGeometry().width(), 24);
    move(0, 0);
}



void PolluxusMain::onTest()
{
    QMetaObject::invokeMethod(pIBAdapter, "onTest", Qt::QueuedConnection);

}

void PolluxusMain::onAdapterConnect()
{
    if(btnConnect->isChecked())
    {
        qDebug() << "MainWindow:Hi I am connecting IB.------"  << QThread::currentThreadId();

        QString host = editHost->text();
        int port = editPort->text().toInt();
        int clientId = editClientId->text().toInt();

        QMetaObject::invokeMethod(pIBAdapter, "onConnect", Qt::QueuedConnection,
                                  Q_ARG(QString, host),
                                  Q_ARG(int, port),
                                  Q_ARG(int, clientId));

        btnConnect->setText(tr("Connecting..."));
        btnConnect->setEnabled(false);


        lbLight->setPixmap(QPixmap(":/images/wait.png"));

    }

    else
    {
        qDebug() << "MainWindow:Hi I am disconnecting ib.------"  << QThread::currentThreadId();

        QMetaObject::invokeMethod(pIBAdapter, "onDisconnect", Qt::QueuedConnection );

        btnConnect->setText(tr("Disconnecting"));
        btnConnect->setEnabled(false);
        lbLight->setPixmap(QPixmap(":/images/bullet-grey.png"));

    }
}

void PolluxusMain::onAdapterConnected()
{
    qDebug() << "MainWindow:Recv connected signal from Posix.------"  << QThread::currentThreadId();

    btnConnect->setText(tr("Disconnect"));
    btnConnect->setEnabled(true);

    editHost->setEnabled(false);
    editPort->setEnabled(false);
    editClientId->setEnabled(false);

    lbLight->setPixmap(QPixmap(":/images/bullet-green.png"));

    pMsgProcessor->start();

    //QMetaObject::invokeMethod(pIBAdapter, "onReqCurrentTime", Qt::QueuedConnection);
}

void PolluxusMain::onAdapterDisconnected()
{
    qDebug() << "MainWindow:Recv disconnected signal from Posix.------"  << QThread::currentThreadId();

    btnConnect->setText(tr("Connect"));
    btnConnect->setEnabled(true);
    editHost->setEnabled(true);
    editPort->setEnabled(true);
    editClientId->setEnabled(true);
    lbLight->setPixmap(QPixmap(":/images/bullet-red.png"));

}


void PolluxusMain::saveWorkSpace()
{
    qDebug() << "PolluxusMain::saveWorkSpace";
    QString iniFileString = QDir::currentPath() + "/workspace.ini";
    QSettings *wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

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

void PolluxusMain::loadWorkSpace()
{
    qDebug() << "PolluxusMain::loadWorkSpace";
    QString iniFileString = QDir::currentPath() + "/workspace.ini";

    QSettings *wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    int port;
    port = wsSettings->value("appname", 4000).toInt();
    qDebug() << "Read successfully ini:" << port;
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
}
