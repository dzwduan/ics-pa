#include <common.h>
#include <sys/time.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int i;
  for(i=0;i<len;i++) {
    putch(((char *)buf)[i]); 
  }
  return len;
}


// return 0 for success, or -1 for failure
// AM_TIMER_UPTIME, AM系统启动时间, 可读出系统启动后的微秒数.
// #define io_read(reg) 
//   ({ reg##_T __io_param; 
//     ioe_read(reg, &__io_param); 
//     __io_param; })
int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  tv->tv_sec = io_read(AM_TIMER_UPTIME).us/1000000;
  tv->tv_usec = io_read(AM_TIMER_UPTIME).us%1000000;
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
