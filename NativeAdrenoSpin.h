/* SPDX-License-Identifier: MIT */
#ifndef NATIVE_ADRENO_SPIN_H
#define NATIVE_ADRENO_SPIN_H
#include <stdint.h>

/* A wait that reads the timebase in a loop instead of blocking on a timer deadline.
 * IODelay and IOSleep are both delay_for_interval(), which asserts its wakeup on the CURRENT
 * core's timer queue (osfmk/kern/clock.c:1605). A core that has stopped taking its timer
 * interrupt never wakes the caller, and the bring-up then stops for good: on 2026-11-08 the
 * board's GPU probe thread sat on `ev=_clock_delay_until` from t=10s to t=830s while its core's
 * timer counters did not move by one. This header has no other include on purpose: the files
 * that need it (the ring's poll, the publish wait) do not carry the GMU headers. */
extern "C" uint64_t mach_absolute_time(void);
extern "C" void nanoseconds_to_absolutetime(uint64_t nanoseconds, uint64_t *result);
static inline void native_adreno_spin_us(unsigned microseconds)
{
    static uint64_t ticksPerUs;
    if(!ticksPerUs) {
        uint64_t one=0;
        nanoseconds_to_absolutetime(1000,&one);
        ticksPerUs=one?one:1;
    }
    const uint64_t deadline=mach_absolute_time()+(uint64_t)microseconds*ticksPerUs;
    while(mach_absolute_time()<deadline) __asm__ volatile("yield" ::: "memory");
}
#endif
