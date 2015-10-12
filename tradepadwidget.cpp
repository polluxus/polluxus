#include "tradepadwidget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpacerItem>

TradePadWidget::TradePadWidget(QWidget *parent) : QWidget(parent)
{
    //iniFileString = QDir::currentPath() + "/workspace.ini";
    //wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    setWindowFlags(Qt::Window);
    setWindowTitle("TradePad");

    QLabel *pLabelSymbol = new QLabel("Contract:");
    pEditSymbol = new QLineEdit();

    QLabel *pLabelAction = new QLabel("Side:");
    pComboAction = new QComboBox();
    pComboAction->addItem("BUY");
    pComboAction->addItem("SELL");

    QLabel *pLabelOrderType = new QLabel("Type:");
    pComboOrderType = new QComboBox();
    pComboOrderType->addItem("LMT");
    pComboOrderType->addItem("MKT");

    QLabel *pLabelQty = new QLabel("Qty:");
    pEditQty = new QLineEdit();

    QLabel *pLabelPrice = new QLabel("Price:");
    pEditPrice = new QLineEdit();

    QWidget* spacer = new QWidget();
    pBtnSubmit = new QPushButton("Submit");
    pBtnSubmit->setStyleSheet("background-color:#006666;");


    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(pLabelSymbol,0,0,1,1);
    gridLayout->addWidget(pEditSymbol,0,1,1,1);
    gridLayout->addWidget(pLabelAction,1,0,1,1);
    gridLayout->addWidget(pComboAction,1,1,1,1);
    gridLayout->addWidget(pLabelOrderType,2,0,1,1);
    gridLayout->addWidget(pComboOrderType,2,1,1,1);

    gridLayout->addWidget(pLabelQty,3,0,1,1);
    gridLayout->addWidget(pEditQty,3,1,1,1);
    gridLayout->addWidget(pLabelPrice,4,0,1,1);
    gridLayout->addWidget(pEditPrice,4,1,1,1);
    gridLayout->addWidget(spacer,5,0,1,1);
    gridLayout->addWidget(pBtnSubmit,5,1,1,1);

    setLayout(gridLayout);

    setMinimumSize(200, 200);

    connect(pComboAction, SIGNAL(currentIndexChanged(QString)), this, SLOT(onActionChanged(QString)));
}

void TradePadWidget::onActionChanged(QString action)
{
    if(action == "BUY")
    {
        pBtnSubmit->setStyleSheet("background-color:#006666;");
    }
    else
    {
        pBtnSubmit->setStyleSheet("background-color:#400000;");
    }
}

void TradePadWidget::onContractClicked(QString symbol)
{
    pEditSymbol->setText(symbol);
}
