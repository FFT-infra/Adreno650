/* SPDX-License-Identifier: MIT */
#ifndef NATIVE_ADRENO_KERNEL_TIME_H
#define NATIVE_ADRENO_KERNEL_TIME_H
#include "NativeAdrenoGmuPlatform.h"

/* Real kernel time for the GMU startup: monotonic microseconds from
 * mach_absolute_time and a bounded busy wait. This is the only production
 * implementation of the time callbacks; tests inject their own. */
struct NativeAdrenoKernelTime {
    static int now_us(void *,uint64_t *);
    static int wait_us(void *,unsigned);
    /* A wait that reads the timebase in a loop instead of blocking on a timer deadline.
     * IODelay and IOSleep are both delay_for_interval(), which asserts its wakeup on the CURRENT
     * core's timer queue (osfmk/kern/clock.c:1605). Timebase spins need no timer interrupt;
     * they are used only for this driver's bounded short waits, not as a boot-stall fix. */
    static void spin_us(unsigned);
};
#endif
