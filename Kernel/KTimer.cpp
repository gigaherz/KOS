#include "Kernel.h"

#define TIMER_MODE_TERMINAL 0  	//Interrupt on terminal count
#define TIMER_MODE_ONESHOT  1 	//Hardware Retriggerable one shot
#define TIMER_MODE_RATEGEN  2 	//Rate Generator
#define TIMER_MODE_SQRWAVE  3 	//Square Wave Mode
#define TIMER_MODE_SSTROBE  4 	//Software Strobe
#define TIMER_MODE_HSTROBE  5 	//Hardware Strobe

UInt32 tickCounter = 0;
UInt32 driftCounter = 0;
UInt32 driftIncrement = 0;

#define TIMER_FREQ 1193180
#define TICK_FREQ 10000

#define NUM_TIMERS 128

struct TimerInfo
{
    UInt32 prev; // linked list
    UInt32 next;

    // Properties
    Bool active;
    UInt32 interval;
    pTimerCallback callback;

    // Status
    UInt32 counter;
    UInt32 prevTicks;

};

TimerInfo timers[NUM_TIMERS];

UInt32 timerFreeHead = NUM_TIMERS;
UInt32 timerBusyHead = NUM_TIMERS;

void KTimerSetup(int channel, int mode, int freq)
{
    int divisor = TIMER_FREQ / freq;
    driftIncrement = TIMER_FREQ % freq;

    int command = ((channel&3)<<6) | ((mode&7)<<1) | 0x30;

    KPortOutputB(0x43, command);          /* Set our command byte */
    KPortOutputB(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    KPortOutputB(0x40, divisor >> 8);     /* Set high byte of divisor */
}

ResultCode __stdcall KTimerInterruptHandler(Regs* regs)
{
    tickCounter++;
    driftCounter += driftIncrement;

    if (driftCounter >= TIMER_FREQ)
    {
        tickCounter++;
        driftCounter -= TIMER_FREQ;
    }

    int c = timerBusyHead;

    while (c < NUM_TIMERS)
    {
        int n=timers[c].next;
        timers[c].counter--;
        if (timers[c].counter<=0)
        {
            // WARNING: This is extremely unsafe.
            // To be replaced with a thread wakeup system and/or auxiliary thread,
            // to avoid stalling the interrupts.
            int diff = tickCounter - timers[c].prevTicks;
            timers[c].prevTicks = tickCounter;
            timers[c].counter = timers[c].interval;
            timers[c].callback(c, diff / (TICK_FREQ/1000));
        }
        c=n;
    }

    return 0;
}

UInt32 KTimerGetTicks()
{
    return tickCounter / (TICK_FREQ/1000);
}

void KTimerInit()
{
    for (int i=0;i<NUM_TIMERS;i++)
    {
        timers[i].prev = i-1;
        timers[i].next = i+1;
    }
    timers[0].prev = NUM_TIMERS; // end of list

    timerFreeHead = 0;
    timerBusyHead = NUM_TIMERS;

    KInterruptRegisterIRQHandler(0, KTimerInterruptHandler);
    KTimerSetup(0,TIMER_MODE_SQRWAVE,TICK_FREQ);
}

ResultCode KTimerAlloc(UInt32 interval, pTimerCallback timerCB, UInt32* timerID)
{
    if (timerFreeHead >= NUM_TIMERS)
        return 1; // MAKE_RESULT(LEVEL_ERROR,CLASS_RESOURCES,CODE_NO_MORE_HANDLES);

    if (interval <= 0)
        return 2;

    if (timerCB == NULL)
        return 3;

    if (timerID == NULL)
        return 4;

    UInt32 tid = *timerID = timerFreeHead;

    // pop from free head
    timerFreeHead = timers[tid].next;
    if (timerFreeHead < NUM_TIMERS)
        timers[timerFreeHead].prev = NUM_TIMERS;

    // push into busy head
    if (timerBusyHead < NUM_TIMERS)
        timers[timerBusyHead].prev = tid;
    timers[tid].next = timerBusyHead;
    timerBusyHead = tid;

    timers[tid].interval = interval * (TICK_FREQ/1000);
    timers[tid].counter = interval;
    timers[tid].callback = timerCB;
    timers[tid].active = True;
    timers[tid].prevTicks = tickCounter;

    return 0;
}

ResultCode KTimerSetInterval(UInt32 timerID, UInt32 interval)
{
    if (timerID < 0 || timerID >= NUM_TIMERS)
        return 1;

    if (!timers[timerID].active)
        return 2;

    if (interval <= 0)
        return 3;

    timers[timerID].interval = interval * (TICK_FREQ/1000);
    if (timers[timerID].counter > timers[timerID].interval)
        timers[timerID].counter = timers[timerID].interval;

    return 0;
}

ResultCode KTimerSetCallback(UInt32 timerID, pTimerCallback timerCB)
{
    if (timerID < 0 || timerID >= NUM_TIMERS)
        return 1;

    if (!timers[timerID].active)
        return 2;

    if (timerCB == NULL)
        return 3;

    timers[timerID].callback = timerCB;

    return 0;
}

ResultCode KTimerRelease(UInt32 timerID)
{
    if (timerID < 0 || timerID >= NUM_TIMERS)
        return 1;

    if (!timers[timerID].active)
        return 2;

    // pop from busy queue
    int prev = timers[timerID].prev;
    int next = timers[timerID].next;

    if (prev < NUM_TIMERS)
        timers[prev].next = next;
    else
        timerBusyHead = next;

    if (next < NUM_TIMERS)
        timers[next].prev = prev;
    //else
    //	timerBusyTail = prev;


    // push into free head
    if (timerFreeHead < NUM_TIMERS)
        timers[timerFreeHead].prev = timerID;
    timers[timerID].next = timerFreeHead;
    timerFreeHead = timerID;

    timers[timerID].interval = 0;
    timers[timerID].counter = 0;
    timers[timerID].callback = NULL;
    timers[timerID].prevTicks = 0;
    timers[timerID].active = False;

    return 0;
}