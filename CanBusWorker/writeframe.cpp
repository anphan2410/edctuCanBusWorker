#include "writeframe.h"

writeFrame::writeFrame(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
    idleTimer.setParent(this);
    idleTimer.setInterval(30000);
    QObject::connect(&idleTimer, &QTimer::timeout, database, &CanBusWorkerDB::sendCanProtocolPresenceRequest);
    TimerFrameWritten.setParent(this);
    TimerFrameWritten.setInterval(3000);
    TimerFrameWritten.setSingleShot(true);
    QObject::connect(&TimerFrameWritten, &QTimer::timeout, this, [&](){
        dbPtr->setError(CanBusWorkerDB::FrameWrittenTimedOut, QStringLiteral(""));
    });
    writeAFrame * substate = new writeAFrame(this,database,&TimerFrameWritten);
    QFinalState * done = new QFinalState(this);    
    FrameSent * signalFrameSent = new FrameSent(database);
    signalFrameSent->setTargetState(done);
    substate->addTransition(signalFrameSent);
    this->setInitialState(substate);
    QObject::connect(done, &QFinalState::entered, this, [&](){idleTimer.start();});
    QObject::connect(done, &QFinalState::exited, this, [&](){idleTimer.stop();});
}

void writeFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
}

void writeFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
}
