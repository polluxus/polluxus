#include "testsignalslot.h"
#include <QDebug>

TestSignalSlot::TestSignalSlot(QObject *parent) : QObject(parent)
{
    pThread = new QThread;
    pThread->start();
    moveToThread( pThread );
}
TestSignalSlot::~TestSignalSlot()
{
    pThread->exit( 0 );
    pThread->wait();
    delete pThread;
}

void TestSignalSlot::onDummySignalTraded(const QString &msg)
{
    qDebug() << "TestSignalSlot::onDummySignalTraded():" << msg;
}

void TestSignalSlot::onDummySignalTicked(const QString &msg)
{
    qDebug() << "TestSignalSlot::onDummySignalTicked():" << msg;
}


