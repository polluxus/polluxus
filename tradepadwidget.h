#ifndef TRADEPADWIDGET_H
#define TRADEPADWIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>

#include <QMainWindow>

class TradePadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TradePadWidget(QWidget *parent = 0);

    QLineEdit *pEditSymbol;
    QComboBox *pComboAction;
    QComboBox *pComboOrderType;

    QLineEdit *pEditQty;
    QLineEdit *pEditPrice;

    QPushButton *pBtnSubmit;

signals:

public slots:
    void onActionChanged(QString action);
    void onContractClicked(QString symbol);
};

#endif // TradePadWidget_H
