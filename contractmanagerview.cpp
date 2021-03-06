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
    iniFileString = QDir::currentPath() + "/workspace.ini";
    wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

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
    connect(pTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));

    pTableView->show();

    createContextMenu();

    setMinimumSize(200, 200);
    loadWorkSpace();
}

ContractManagerView::~ContractManagerView()
{
    saveWorkSpace();
    if(!pModel) delete pModel;
    if(!wsSettings) delete wsSettings;
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
    connect(pAtnDelete,SIGNAL(triggered()), this, SLOT(onAtnDeleteTriggered()));
}

void ContractManagerView::saveWorkSpace()
{
    //qDebug() << "ContractManagerView::saveWorkSpace()";
    //qDebug() << "pos():" << pos();
    //qDebug() << "FrameGeo:" << this->frameGeometry();

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
    //qDebug() << "ContractManagerView::loadWorkSpace()";

    wsSettings->beginGroup("contractmanager");
    restoreGeometry(wsSettings->value( "geometry", saveGeometry() ).toByteArray());

    move(wsSettings->value( "pos", pos() ).toPoint());
    resize(wsSettings->value( "size", size()).toSize());
    if ( wsSettings->value( "maximized", isMaximized() ).toBool() )
    {
        showMaximized();
    }
    wsSettings->endGroup();

    //qDebug() << "pos():" << pos();
    //qDebug() << "FrameGeo:" << this->frameGeometry();



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

       // qDebug()<<"pContextMenu index row(): " << index.row();

    }
}

void ContractManagerView::onAtnSubscribeTriggered()
{
    //Check status
    QModelIndex selIndex = pTableView->selectionModel()->currentIndex();
    int idxRow = selIndex.row();


    QString symbol = pModel->index(idxRow,0).data().toString();
    int nCol = pModel->columnCount();

    qDebug()<<"symbol:"<<symbol;

    if(pModel->headerData(idxRow, Qt::Vertical, Qt::DisplayRole).toString()=="OFF")
    {
        emit pModel->SubscribeMktData(symbol);
        pModel->mGridData[symbol][nCol] = "ON";

    }
    else
    {
        emit pModel->UnsubscribeMktData(symbol);
        pModel->mGridData[symbol][nCol] = "OFF";
    }

    emit pModel->headerDataChanged(Qt::Vertical, idxRow, idxRow);
}

void ContractManagerView::onAtnDeleteTriggered()
{
    //Check status
//    QModelIndex selIndex = pTableView->selectionModel()->currentIndex();
//    int idxRow = selIndex.row();


//    QString contractId = pModel->index(idxRow,0).data().toString();
//    QString exchange = pModel->index(idxRow,1).data().toString();
//    QString subStatus = pModel->index(idxRow,11).data().toString();

//    qDebug()<<"contractId:"<<contractId<<"exchange:"<< exchange <<"Status:" << subStatus;

//    if(subStatus == "ON")
//    {
//        emit UnsubscribeMktData(contractId);
//    }

//    pModel->mGridData.remove(contractId);
//    pModel->mKeyList.removeAll(contractId);
//    pModel->removeRow(idxRow);
//    emit pModel->layoutChanged();

}

void ContractManagerView::onClicked(QModelIndex index)
{
    if(index.row()>=0 && index.row()< pModel->rowCount()-1)
    {
        //pTableView->clearSelection();
        //pTableView->selectRow(index.row());
        int nCol = pModel->columnCount();
        QString symbol;
        symbol = (pModel->mKeyList).at(index.row());
        if(pModel->mGridData[symbol][nCol] == "ON")
        {
            QString bidSz = "";
            QString bid = "";
            QString ask = "";
            QString askSz = "";

            bidSz = pModel->mGridData[symbol][2];
            bid = pModel->mGridData[symbol][3];
            ask = pModel->mGridData[symbol][4];
            askSz = pModel->mGridData[symbol][5];

            emit ContractClicked(symbol, bidSz, bid, ask, askSz);
        }
    }
}




