#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "posixibclient.h"

class MessageProcessor : public QThread
{
    Q_OBJECT
public:
    MessageProcessor(PosixIBClient *pIBClient, QObject *parent = 0);
    ~MessageProcessor();
public slots:

private:
    void run()
    {
        qDebug() << "MessageProcessor: start() in thread: " << QThread::currentThreadId();
        while (pIBClient->isConnected())
        {
            //qDebug() << "MessageProcessor: start() in thread: " << QThread::currentThreadId();
            pIBClient->processMessages();
        }
        //for some reason, data feed connect not available
        pIBClient->emitAdapterDisconnected();
    }

private:
    PosixIBClient *pIBClient;
};

#endif // MESSAGEPROCESSOR_H
