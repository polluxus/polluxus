#include "contractmanager.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QPushButton>
#include <QToolBar>

ContractManager::ContractManager(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("ContractManager");

    createToolBar();


    pTableView = new QTableView();
    pModel = new ContractManagerModel(0);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(pToolBar);
    vLayout->addWidget(pTableView);

    setLayout(vLayout);

    pTableView->setModel(pModel);
    pTableView->show();

    setMinimumSize(320, 200);
    loadWorkSpace();
}

ContractManager::~ContractManager()
{
    saveWorkSpace();
}

void ContractManager::createToolBar()
{
    pToolBar = new QToolBar;
    btnSubscribe = new QPushButton(tr("Subscribe"));
    pToolBar->addWidget(btnSubscribe);
}

void ContractManager::saveWorkSpace()
{
    qDebug() << "ContractManager::saveWorkSpace()";
    qDebug() << "pos():" << pos();
    qDebug() << "FrameGeo:" << this->frameGeometry();




    QString iniFileString = QDir::currentPath() + "/workspace.ini";

    QSettings *wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    wsSettings->beginGroup("contractmanager");
    wsSettings->setValue("geometry", saveGeometry());
    wsSettings->setValue( "maximized", isMaximized());
    if ( !isMaximized() ) {
            wsSettings->setValue( "pos", pos() + QPoint(0,8));
            wsSettings->setValue( "size", size() );
        }
    wsSettings->endGroup();

    wsSettings->sync();
}

void ContractManager::loadWorkSpace()
{
    qDebug() << "ContractManager::loadWorkSpace()";


    QString iniFileString = QDir::currentPath() + "/workspace.ini";

    QSettings *wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    wsSettings->beginGroup("contractmanager");
    restoreGeometry(wsSettings->value( "geometry", saveGeometry() ).toByteArray());

    move(wsSettings->value( "pos", pos() ).toPoint());
    resize(wsSettings->value( "size", size()).toSize());
    if ( wsSettings->value( "maximized", isMaximized() ).toBool() )
    {
        showMaximized();
    }
    wsSettings->endGroup();

    qDebug() << "pos():" << pos();
    qDebug() << "FrameGeo:" << this->frameGeometry();



}

void ContractManager::onTickUpdating(const Tick &tick)
{
    qDebug() << "Recieved TickUpdating()"<<tick.contractId;
    pModel->onTickUpdating(tick);

}
