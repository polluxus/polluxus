#ifndef POLLUXUSUTILITY_H
#define POLLUXUSUTILITY_H
#include <QMutex>
#include <QtGlobal>

class PolluxusUtility
{
public:
    static long baseValidId;
    static long getNextValidId();
    static void registerMetaType();
private:
    static QMutex mutex;
};

#endif // POLLUXUSUTILITY_H
