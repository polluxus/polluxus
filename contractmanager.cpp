#include "contractmanager.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QSettings>

ContractManager::ContractManager(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("ContractManager");

    pTableView = new QTableView();
    pModel = new ContractManagerModel(0);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

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

void ContractManager::saveWorkSpace()
{
    qDebug() << "saveWorkSpace";
    QString iniFileString = QDir::currentPath() + "/workspace.ini";

    QSettings *wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    wsSettings->beginGroup("contractmanager");
    wsSettings->setValue("geometry", saveGeometry());
    wsSettings->setValue( "maximized", isMaximized());
    if ( !isMaximized() ) {
            wsSettings->setValue( "pos", pos() );
            wsSettings->setValue( "size", size() );
        }
    wsSettings->endGroup();

    wsSettings->sync();
}

void ContractManager::loadWorkSpace()
{
    qDebug() << "loadWorkSpace";
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
}

