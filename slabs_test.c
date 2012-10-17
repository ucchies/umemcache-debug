#include <stdio.h>
#include <time.h>

/* #ifndef bool */
/* #define bool int  */
/* #endif */

/* #include "memcached.h" */
/* #include "slabs.h" */

#define TESTTIME 10000

/* 
 * Required slabs_lock
 */
unsigned int spare_larger_clsid(unsigned int *id) {
    unsigned int res = *id;
    if (id == 0) return 0;    
    while (slabclass[res].sl_curr == 0) {
        if (res++ == power_largest) return 0;
    }
    *id = res;
    return 1;
}


int main(int argc, char *argv[]) {
  unsigned int clsid = 1;
  time_t start_time, end_time;
  int exectime;
  
  time(&start_time);
  int i;
  for (i=0; i < TESTTIME; i++) {
    spare_larger_clsid(&clsid);
  }
  time(&end_time);
  exectime = end_time - start_time;

  printf("Test: %d, Time: %d", TESTTIME, exectime);
}