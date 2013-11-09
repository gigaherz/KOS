#pragma once

#include "KTypes.h"
#include "KConstants.h"
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
#include "KObject.h"

#include "KPool.h"

#include "KAvlTree.h"

#include "KQueue.h"

#include "Multiboot.h"

// Todo later

// Function: KThreadGetCurrentThreadId
// Description:
//    Returns the ID for the currently executing thread.
//    If the thread is a kernel thread, the top bit of the ID will be set.
// Return Value: The ID for the currently executing thread.
Int32 KThreadGetCurrentThreadId();

ResultCode KThreadSuspend(Int32 threadId);
ResultCode KThreadResume(Int32 threadId);


void KThreadYield();