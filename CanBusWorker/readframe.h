#ifndef READFRAME_H
#define READFRAME_H

#include <QState>
#include <QTime>
#include <QTimer>
#include "canbusworkerdb.h"

class readFrame : public QState
{
public:
    readFrame(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
    QTimer idleTimer;
};

#endif // READFRAME_H
