#pragma once

//////////////////////////////////////////////////////////////////////////
// Timer
void KTimerInit();
UInt32  KTimerGetTicks();

typedef Bool (* pTimerCallback)(UInt32 timerID, UInt32 tickDiff);

ResultCode KTimerAlloc(UInt32 interval, pTimerCallback timerCB, UInt32* timerID);
ResultCode KTimerSetInterval(UInt32 timerID, UInt32 interval);
ResultCode KTimerSetCallback(UInt32 timerID, pTimerCallback timerCB);
ResultCode KTimerRelease(UInt32 timerID);