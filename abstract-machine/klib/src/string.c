#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *p = s;
  int len=0;
  while(*p++) len++;
  return len;
} 

char *strcpy(char* dst,const char* src) {
  char * d = dst;
  const char *s = src;
  while((*d++ = *s++));
  *d='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char * d = dst;
  while(n-- && (*d++ = *src++));
  *d='\0';
  return dst;
}

//dst+src
char* strcat(char* dst, const char* src) {
  char * d = dst;
  while(*d) d++; //如果++放在里面，那么d='\0'时还会++
  while((*d++ = *src++)) ;
  *d = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  const char * p = s1;
  const char * q = s2;
  int flag = 0;
  while(*p++ && *q++){
    if(*p==*q) continue;
    else if(*p > *q) {
      flag = 1; break;
    }
    else{
      flag = -1; break;
    } 
  }
  return flag;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  const char * p = s1;
  const char * q = s2;
  int flag = 0;
  while(*p++ && *q++){
    if(*p==*q) continue;
    else if(*p > *q) {
      flag = 1; break;
    }
    else{
      flag = -1; break;
    } 
  }
  return flag;
}

void* memset(void* v,int c,size_t n) {
  char * addr = (char *)v;
  while(n--)
    *addr++ = c;
  *addr = '\0';
  return v;
}

//需要考虑数据重叠
void* memmove(void* dst,const void* src,size_t n) {
  char * d = (char*)dst;
  const char * s = (char*)src;
  while(n-- && (*d++ = *s++)!='\0');
  *d = '\0';
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  char * d = (char*)out;
  const char * s = (char*)in;
  while(n-- && (*d++ = *s++)!='\0');
  *d = '\0';
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  char * p = (char*)s1;
  const char * q = (char*)s2;
  int flag = 0;
  while(n-- && *p && *q){
    if(*p==*q) continue;
    else if(*p > *q) flag = 1;
    else flag = -1;
  }
  return flag;
}
#endif
