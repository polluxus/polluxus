#include "contractmanagerview.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QPushButton>
#include <QToolBar>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

ContractManagerView::ContractManagerView(QWidget *parent) : QWidget(parent)
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

    (pTableView->horizontalHeader())->setSectionResizeMode(QHeaderView::ResizeToContents);
    pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pTableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomMenuRequested(QPoint)));

    pTableView->show();

    createContextMenu();

    setMinimumSize(320, 200);
    loadWorkSpace();
}

ContractManagerView::~ContractManagerView()
{
    saveWorkSpace();
}

void ContractManagerView::createToolBar()
{
    pToolBar = new QToolBar;
    btnSubscribe = new QPushButton(tr("Subscribe"));
    pToolBar->addWidget(btnSubscribe);
}

void ContractManagerView::createContextMenu()
{
    pAtnSubscribe = new QAction("Toggle Subscribe", this);
    pAtnDelete = new QAction("Delete", this);
    pAtnDetail = new QAction("Contract Detail...", this);

    pContextMenu = new QMenu(this);
    pContextMenu->addAction(pAtnDetail);
    pContextMenu->addSeparator();
    pContextMenu->addAction(pAtnSubscribe);
    pContextMenu->addAction(pAtnDelete);

    connect(pAtnSubscribe,SIGNAL(triggered()), this, SLOT(onAtnSubscribeTriggered()));

}

void ContractManagerView::saveWorkSpace()
{
    qDebug() << "ContractManagerView::saveWorkSpace()";
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

void ContractManagerView::loadWorkSpace()
{
    qDebug() << "ContractManagerView::loadWorkSpace()";


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

void ContractManagerView::onTickUpdating(const Tick &tick)
{
    qDebug() << "Recieved TickUpdating()"<<tick.contractId;
    pModel->onTickUpdating(tick);

}



void ContractManagerView::onCustomMenuRequested(QPoint pos)
{
    QModelIndex index = pTableView->indexAt(pos);

    //popup menu only in range
    if(index.row()>=0 && index.row()< pModel->rowCount()-1)
    {
        //pTableView->clearSelection();
        //pTableView->selectRow(index.row());
        pContextMenu->popup(pTableView->viewport()->mapToGlobal(pos));

        qDebug()<<"pContextMenu index row(): " << index.row();

    }
}

void ContractManagerView::onAtnSubscribeTriggered()
{
    //QModelIndex index = pTableView->indexAt(pContextMenu->pos());
    qDebug()<<"CurrentIndex row()" << pTableView->selectionModel()->currentIndex().row();
    //Check status
    QModelIndex selIndex = pTableView->selectionModel()->currentIndex();
    int idxRow = selIndex.row();

    QString subStatus = pModel->index(idxRow,11).data().toString();
    qDebug()<<"Status:" << subStatus;

    if(subStatus == "ON")
    {

    }
    else
    {

    }
}



