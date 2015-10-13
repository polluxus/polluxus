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

    pLabelSymbol = new QLabel("Symbol");

    pBtnBid = new QPushButton("12.32");
    pBtnAsk = new QPushButton("12.34");
    pLabelBidSz = new QLabel("150");
    pLabelAskSz = new QLabel("132");

    pLabelSymbol->setStyleSheet("background-color: #000033;"
                               "font: 18pt;"
                               "border: 1px solid #808080;"
                               "border-radius: 5px;"
                               "qproperty-alignment: AlignCenter;");

    pBtnBid->setStyleSheet("background-color: #006666;"
                             "font: 24pt;"
                             "border: 1px solid #010000;"
                             "border-radius: 5px;"
                             "qproperty-alignment: AlignCenter;");

    pLabelBidSz->setStyleSheet("background-color: #006666;"
                               "font: 16pt;"
                               "border: 1px solid #010000;"
                               "border-radius: 5px;"
                               "qproperty-alignment: AlignCenter;");

    pBtnAsk->setStyleSheet("background-color: #400000;"
                             "font: 24pt;"
                             "border: 1px solid #500000;"
                             "border-radius: 5px;"
                             "qproperty-alignment: AlignCenter;");

    pLabelAskSz->setStyleSheet("background-color: #400000;"
                               "font: 16pt;"
                               "border: 1px solid #500000;"
                               "border-radius: 5px;"
                               "qproperty-alignment: AlignCenter;");


    pComboAction = new QComboBox();
    pComboAction->addItem("BUY");
    pComboAction->addItem("SELL");

    pComboOrderType = new QComboBox();
    pComboOrderType->addItem("LMT");
    pComboOrderType->addItem("MKT");

    QLabel *pLabelQty = new QLabel("Qty:");
    pEditQty = new QLineEdit();


    QLabel *pLabelPrice = new QLabel("Price:");
    pEditPrice = new QLineEdit();

    //QWidget* spacer = new QWidget();
    pBtnSubmit = new QPushButton("BUY");
    pBtnSubmit->setStyleSheet("background-color:#006666;font: 18pt;");
    pBtnSubmit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(pLabelSymbol,0,0,1,2);

    gridLayout->addWidget(pBtnAsk,1,0,1,1);
    gridLayout->addWidget(pLabelAskSz,1,1,1,1);

    gridLayout->addWidget(pBtnBid,2,0,1,1);
    gridLayout->addWidget(pLabelBidSz,2,1,1,1);

    gridLayout->addWidget(pComboAction,3,0,1,1);
    gridLayout->addWidget(pComboOrderType,3,1,1,1);

    gridLayout->addWidget(pLabelQty,4,0,1,1);
    gridLayout->addWidget(pEditQty,4,1,1,1);

    gridLayout->addWidget(pLabelPrice,5,0,1,1);
    gridLayout->addWidget(pEditPrice,5,1,1,1);

    gridLayout->addWidget(pBtnSubmit,6,0,1,2);
    setLayout(gridLayout);

    setMinimumSize(200, 200);

    connect(pComboAction, SIGNAL(currentIndexChanged(QString)), this, SLOT(onActionChanged(QString)));
    connect(pComboOrderType, SIGNAL(currentIndexChanged(QString)), this, SLOT(onOrderTypeChanged(QString)));
    connect(pBtnBid, SIGNAL(clicked(bool)), this, SLOT(onBtnBidClicked(bool)));
    connect(pBtnAsk, SIGNAL(clicked(bool)), this, SLOT(onBtnAskClicked(bool)));
    connect(pBtnSubmit, SIGNAL(clicked(bool)),this, SLOT(onSubmit(bool)));
}

void TradePadWidget::onActionChanged(QString action)
{
    if(action == "BUY")
    {
        pBtnSubmit->setStyleSheet("background-color:#006666;");
        pBtnSubmit->setText("BUY");
    }
    else
    {
        pBtnSubmit->setStyleSheet("background-color:#400000;");
        pBtnSubmit->setText("SELL");
    }
}

void TradePadWidget::onOrderTypeChanged(QString orderType)
{
    if(orderType=="MKT")
    {
        pEditPrice->setText("0");
        pEditPrice->setEnabled(false);
    }
    else
    {
        QString price = "";
        if(pComboAction->currentText() == "BUY")
            price = pBtnBid->text();
        else
            price = pBtnAsk->text();

        pEditPrice->setText(price);
        pEditPrice->setEnabled(true);
    }
}

void TradePadWidget::onBtnBidClicked(bool btnStatus)
{
    if(pEditPrice->isEnabled())
    {
        pEditPrice->setText(pBtnBid->text());
    }
}

void TradePadWidget::onBtnAskClicked(bool btnStatus)
{
    if(pEditPrice->isEnabled())
    {
        pEditPrice->setText(pBtnAsk->text());
    }
}

void TradePadWidget::onContractClicked(QString symbol, QString bidSz, QString bid, QString ask, QString askSz)
{
    pLabelSymbol->setText(symbol);
    pLabelBidSz->setText(bidSz);
    pBtnBid->setText(bid);
    pLabelAskSz->setText(askSz);
    pBtnAsk->setText(ask);
}

void TradePadWidget::onInstrumentTicked(const Tick &tick)
{
    if(tick.contractId == pLabelSymbol->text().trimmed())
    {
        switch (tick.typeId.toInt())
        {
            case 0:
                pLabelBidSz->setText(tick.value);
                break;
            case 1:
                pBtnBid->setText(tick.value);
                break;
            case 2:
                pBtnAsk->setText(tick.value);
                break;
            case 3:
                pLabelAskSz->setText(tick.value);
                break;
            default:
                break;
        }
    }
}

void TradePadWidget::onSubmit(bool btnStatus)
{
    //send order to IB
    QString symbol = pLabelSymbol->text();
    QString action = pComboAction->currentText();
    QString orderType = pComboOrderType->currentText();
    int qty= pEditQty->text().toInt();
    double price = pEditPrice->text().toDouble();
    QString strategyId = "polluxus";

    emit OrderSubmit(symbol, action, orderType, price, qty, strategyId);
    pEditQty->setText("");
}
