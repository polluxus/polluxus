#ifndef TRADEPADWIDGET_H
#define TRADEPADWIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>

#include <QMainWindow>

#include "marketdata.h"

class TradePadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TradePadWidget(QWidget *parent = 0);

    QLabel *pLabelSymbol;

    QPushButton *pBtnBid;
    QPushButton *pBtnAsk;
    QLabel *pLabelBidSz;
    QLabel *pLabelAskSz;

    QComboBox *pComboAction;
    QComboBox *pComboOrderType;

    QLineEdit *pEditQty;
    QLineEdit *pEditPrice;

    QPushButton *pBtnSubmit;

signals:
    void OrderSubmit(QString symbol, QString action, QString orderType, double price, int qty, QString strategyId);

public slots:
    void onActionChanged(QString action);
    void onOrderTypeChanged(QString orderType);
    void onBtnBidClicked(bool btnStatus);
    void onBtnAskClicked(bool btnStatus);
    void onContractClicked(QString symbol, QString bidSz, QString bid, QString ask, QString askSz);
    void onInstrumentTicked(const Tick &tick);

    void onSubmit(bool btnStatus);
};

#endif // TradePadWidget_H
