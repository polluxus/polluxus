#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QLCDNumber>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT

public:
    DigitalClock(QWidget *parent = 0);
    void setTimeDiffMS(qint64 timeDiffMS);
private:
    //QThread *pThread;
    qint64 timeDiffMS;
private slots:

    void showTime();
};

#endif // DIGITALCLOCK_H
