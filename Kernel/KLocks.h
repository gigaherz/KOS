#pragma once

void KSpinLock(Int32 volatile * lock);
void KSpinUnlock(Int32 volatile * lock);

void KSeqLockedWriteEnter(Int32 volatile *lock, Int32 volatile *counter);
void KSeqLockedWriteExit(Int32 volatile *lock, Int32 volatile *counter);

// returns Pre-Count
void KSeqLockedReadBegin(Int32 volatile *lock, Int32 volatile *counter, Int32 volatile *pre_count);

// returns True if the read is consistent
Bool KSeqLockedReadEnd(Int32 volatile *lock, Int32 volatile *counter, Int32 volatile *pre_count);
