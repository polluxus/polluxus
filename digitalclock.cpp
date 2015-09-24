#include <QtWidgets>

#include "digitalclock.h"

DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{


    //pThread = new QThread;
    //pThread->start();
    //this->moveToThread(pThread);

    setWindowFlags(Qt::FramelessWindowHint);
    setSegmentStyle(Flat);

    timeDiffMS = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    setDigitCount(10);
    showTime();
    resize(180, 30);


}

void DigitalClock::setTimeDiffMS(qint64 timeDiffMS)
{
    this->timeDiffMS = timeDiffMS;
}

void DigitalClock::showTime()
{
//    QTime time = QTime::currentTime();
//    QString text = time.toString("hh:mm:ss");

    QString text = QDateTime::currentDateTime().addMSecs(-timeDiffMS).toString("hh:mm:ss");
    display(text);
}
