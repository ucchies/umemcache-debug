#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


/* #ifndef bool */
/* #define bool int  */
/* #endif */

/* #include "memcached.h" */
/* #include "slabs.h" */

#define MAX_NUMBER_OF_SLAB_CLASSES 200

typedef struct {
    unsigned int size;      /* sizes of items */
    unsigned int perslab;   /* how many items per slab */

    void *slots;           /* list of item ptrs */
    unsigned int sl_curr;   /* total free items in list */

    unsigned int slabs;     /* how many slabs were allocated for this class */

    void **slab_list;       /* array of slab pointers */
    unsigned int list_size; /* size of prev array */

    unsigned int killing;  /* index+1 of dying slab, or zero if none */
    size_t requested; /* The number of requested bytes */
} slabclass_t;

static slabclass_t slabclass[MAX_NUMBER_OF_SLAB_CLASSES];


#define TESTTIME 1000000
#define power_largest MAX_NUMBER_OF_SLAB_CLASSES

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

void wait(unsigned int *id) {
  int i;
  for (i=0; i < TESTTIME; i++) {
    *id = rand()%power_largest + 1;
  }
}


int main(int argc, char *argv[]) {
  unsigned int clsid = 1;
  /* time_t start_time, end_time; */
  /* int exectime; */
  
  /* time(&start_time); */
  struct timeval start, end;
  double spare_larger_time, for_time, purity_time;
  int i;
  
  gettimeofday(&start, NULL);
  for (i=0; i < TESTTIME; i++) {
    spare_larger_clsid(&clsid);
    clsid = rand()%power_largest + 1;
  }
  gettimeofday(&end, NULL);
  spare_larger_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*1.0E-6);

  gettimeofday(&start, NULL);
  wait(&clsid);
  gettimeofday(&end, NULL);
  for_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*1.0E-6);
  /* time(&end_time); */
  /* exectime = end_time - start_time; */
  purity_time = spare_larger_time - for_time;

  printf("Test: %d, Time: %lf\n", TESTTIME, purity_time);
}
