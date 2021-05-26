#include <am.h>
#include <nemu.h>
// #include <sys/time.h>
// #include <time.h>

uint32_t high,low;

void __am_timer_init() {
    high = inl(0xa1000048);
    low  = inl(0xa100004c);
}

// static struct timeval boot_time = {};

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
    uptime->us = high * 1000000 + (low + 500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
