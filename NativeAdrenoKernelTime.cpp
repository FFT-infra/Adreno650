/* SPDX-License-Identifier: MIT */
#include "NativeAdrenoKernelTime.h"
#include <IOKit/IOLib.h>
#include "NativeAdrenoSpin.h"
#include <kern/clock.h>
int NativeAdrenoKernelTime::now_us(void *cookie,uint64_t *value)
{
    if(cookie || !value)return 1;
    uint64_t nanoseconds=0;
    absolutetime_to_nanoseconds(mach_absolute_time(),&nanoseconds);
    *value=nanoseconds/1000;
    return 0;
}
void NativeAdrenoKernelTime::spin_us(unsigned microseconds)
{
    native_adreno_spin_us(microseconds);
}
int NativeAdrenoKernelTime::wait_us(void *cookie,unsigned microseconds)
{
    if(cookie)return 1;
    spin_us(microseconds);
    return 0;
}
