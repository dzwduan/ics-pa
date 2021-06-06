#include <common.h>
#include "syscall.h"

extern void yield();

enum {
  SYS_exit,
  SYS_yield,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};


size_t _write(int fd, void *buf, size_t count);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;  //第一个参数是系统调用号
  a[1] = c->GPR2;  // a0
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_exit:  halt(a[1]); break;
    case SYS_yield: c->GPRx = 0;  yield(); break;
    case SYS_write: c->GPRx =  _write(a[1],(void *)a[2],a[3]); break;
    case SYS_brk  : c->GPRx = 0; break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}


size_t _write(int fd, void *buf, size_t count){
  //size_t ret = ramdisk_write(buf,0,count);
  size_t ret = count;
  if(fd == 1 || fd == 2) {
    int i=0;

    for(i=0;i<count;i++) {
      putch(*(char *)(buf+i));
    }
    return ret;
  }
  return -1;
}

