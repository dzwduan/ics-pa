#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <NDL.h>


int main() {
  struct timeval time; 
  time.tv_sec = 0;
  printf("start tv_sec = %ld\n",time.tv_sec);
  int cnt = 0;
  while(1)
  { 
      gettimeofday(&time,NULL);
      if(time.tv_usec == 500000){
          printf("pass 0.5s\n");
          time.tv_usec = 0;
      }
  } 
}
