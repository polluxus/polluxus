#include "ordermanagerview.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QHeaderView>

OrderManagerView::OrderManagerView(QWidget *parent) : QWidget(parent)
{
    iniFileString = QDir::currentPath() + "/workspace.ini";
    wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    setWindowFlags(Qt::Window);
    setWindowTitle("OrderManager");


    pTableView = new QTableView();
    pModel = new OrderManagerModel(0);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(pTableView);

    setLayout(vLayout);

    pTableView->setModel(pModel);

    (pTableView->horizontalHeader())->setSectionResizeMode(QHeaderView::ResizeToContents);
    pTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    pTableView->show();

    setMinimumSize(200, 200);
    loadWorkSpace();
}

OrderManagerView::~OrderManagerView()
{
    saveWorkSpace();
    if(!pModel) delete pModel;
    if(!wsSettings) delete wsSettings;
}

void OrderManagerView::saveWorkSpace()
{
    //qDebug() << "ContractManagerView::saveWorkSpace()";
    //qDebug() << "pos():" << pos();
    //qDebug() << "FrameGeo:" << this->frameGeometry();

    wsSettings->beginGroup("ordermanager");
    wsSettings->setValue("geometry", saveGeometry());
    wsSettings->setValue( "maximized", isMaximized());
    if ( !isMaximized() ) {
            wsSettings->setValue( "pos", pos() + QPoint(0,8));
            wsSettings->setValue( "size", size() );
        }
    wsSettings->endGroup();

    wsSettings->sync();
}

void OrderManagerView::loadWorkSpace()
{
    //qDebug() << "ContractManagerView::loadWorkSpace()";

    wsSettings->beginGroup("ordermanager");
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
