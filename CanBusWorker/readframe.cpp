#include "readframe.h"

readFrame::readFrame(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
    idleTimer.setParent(this);
    idleTimer.setInterval(30000);
    QObject::connect(&idleTimer, &QTimer::timeout, database, &CanBusWorkerDB::sendCanProtocolPresenceRequest);
}

void readFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
    while (dbPtr->currentDev->framesAvailable())
    {
        QCanBusFrame tmpFirst = dbPtr->currentDev->readFrame();
        qApp->processEvents();
        CanBusWorkerDB::CanBusFrameWithTimeStamp tmp(tmpFirst,
                                                     QTime::currentTime().toString(QStringLiteral("hh:mm:ss")));
        emit dbPtr->Out(QVariant::fromValue(CanBusWorkerDB::replyCanFrameWithTimeStamp),
                        QVariant::fromValue(tmp));
    }
    idleTimer.start();
}

void readFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
    idleTimer.stop();
}
