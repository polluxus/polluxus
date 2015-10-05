#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QThread>

class OrderManager : public QObject
{
    Q_OBJECT
public:
    explicit OrderManager(QObject *parent = 0);
    virtual ~OrderManager();

    QThread *pThread;
signals:

public slots:
};

#endif // ORDERMANAGER_H
