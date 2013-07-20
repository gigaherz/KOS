#pragma once

#include "KTypes.h"
#include "KInterlocked.h"
#include "KLocks.h"
#include "KRuntime.h"
#include "KDebug.h"
#include "KCpu.h"
#include "KSerial.h"
#include "KKeyboard.h"
#include "KDisplay.h"
#include "KMemory.h"
#include "KTimer.h"
#include "KInterrupt.h"
#include "KRealTimeClock.h"

#include "KQueue.h"

// Todo later
Int32 KThreadGetCurrentThreadId();
void KThreadSuspend(Int32 threadId);
void KThreadResume(Int32 threadId);
