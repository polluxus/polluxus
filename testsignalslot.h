#ifndef TESTSIGNALSLOT_H
#define TESTSIGNALSLOT_H

#include <QObject>
#include <QThread>

class TestSignalSlot : public QObject
{
    Q_OBJECT
public:
    explicit TestSignalSlot(QObject *parent = 0);
    virtual ~TestSignalSlot();

    QThread *pThread;
signals:

public slots:
    void onDummySignalTraded(const QString &msg);
    void onDummySignalTicked(const QString &msg);
};

#endif // TESTSIGNALSLOT_H
