#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <assert.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

//num to string
char * i2a(int n,int radix) { 
    printf("func i2a started\n");
    char digit[] ="0123456789ABCDEF";
    char res[16];
    char *ret = res;
    char *p = res;
    char *h = res;
    if(n==0) {
        *p++ ='0';
        *p = '\0';
        return ret;
    }
    if(n<0 && radix==10) {
        *p++ = '-';
        n = -n;
    }
    if(n<0 && radix!=0) {
        return 0;
    }

    while(n) {
        *p++ = digit[n%radix];
        n/=radix;
    }
    *p = '\0';
    //printf("ret : %s\n",ret);
    //这里不能使用!=,如果数字个数是偶数就会失败
    for(--p;h<p;h++,p--){
        char tmp = *h;
        *h = *p;
        *p = tmp;
    }
    //printf("in func i2a %s \n",res);
    return ret;
}

int print(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  char tmp[128];
  char *p = tmp;
  int ret = vsprintf(tmp,fmt,ap);
  while(*p) {
    putchar(*p);
    p++;
  }
  return ret;
}

int vsprint(char *out, const char *fmt, va_list ap) {
  int len = 0;
  //va_start(ap,fmt);
  char *p = out; //结束后out不能改变
  int n;
  char *s;
  while(*fmt) {
    if(*fmt=='%'){
      switch(*++fmt) {
        case 'd':
          n = va_arg(ap,int);
          s = i2a(n,10);
          strcat(p,s);
          len += strlen(s);
          p   += strlen(s);
          break;
        case 'x':
          n = va_arg(ap,int);
          s = i2a(n,16);
          strcat(p,s);
          len += strlen(s);
          p   += strlen(s);
          break;
        case 'c':
          n = va_arg(ap,int);
          *p++ = n+'0';
          len++;
          break;
        case 's':
          s = va_arg(ap,char *);
          strcat(out,s);
          printf("in s : %s\n",p);
          len += strlen(s);
          p   += strlen(s);
          break;
        default: assert(0);
      }
    }
    else{
      *p++ = *fmt;
      len++;
    }
    fmt++;
  }

  *p = '\0';
  va_end(ap);
  return len;
}

int sprintf(char *out, const char *fmt, ...) {
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
