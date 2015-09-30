#include "orderbookwidget.h"
#include <QPlainTextEdit>
#include <QVBoxLayout>

OrderBookWidget::OrderBookWidget(QWidget *parent) : QWidget(parent)
{

    setWindowFlags(Qt::Window);
    setWindowTitle("OrderBook");


    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(new QPlainTextEdit());

    setLayout(vLayout);
    setMinimumSize(220, 200);
}

