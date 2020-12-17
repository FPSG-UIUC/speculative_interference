
#ifndef UTIL_H_
#define UTIL_H_

#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

#define NUMBER_OF_LOGICAL_CORES 8

/*
 * Cache hierarchy characteristics
 * 
 * To understand these concepts visually, see the presentations here:
 *  https://cs.adelaide.edu.au/~yval/Mastik/
 * 
 * The cache line/block size in my machine is 64 bytes (2^6), meaning that the
 * rightmost 6 bits of the physical address are used as cache block offset. 
 * 
 * The L1 cache in my machine has 64 cache sets (2^6)
 * (cat /sys/devices/system/cpu/cpu0/cache/index0/number_of_sets).
 * That means that the next 6 bits after the block offset of the physical
 * address are used as L1 cache set index.
 * 
 * The LLC cache in my machine has instead 8196 cache sets (2^13) (cat
 * /sys/devices/system/cpu/cpu0/cache/index3/number_of_sets).
 * However these sets are split between a number of slices, which seem to be 8
 * in my CPU.
 * Each slice has the same number of sets (see [1, 2]).
 * That means that each slice has 1024 (2^10) cache sets.
 * Therefore in my CPU, the next 10 bits of the physical address after the block
 * offset are used as LLC cache set index within each slice.
 *
 * [1]: Prime+Abort: A Timer-Free High-Precision L3 Cache Attack using Intel TSX
 * [2]: https://www.anandtech.com/show/3922/intels-sandy-bridge-architecture-exposed/4
 */

#define CACHE_BLOCK_SIZE 64 // Cache block and cache line are the same thing
#define CACHE_BLOCK_SIZE_LOG 6

#define L1_CACHE_WAYS 8
#define L1_CACHE_SETS 64
#define L1_CACHE_SETS_LOG 6
#define L1_CACHE_SIZE (L1_CACHE_SETS) * (L1_CACHE_WAYS) * (CACHE_BLOCK_SIZE)

#define L2_CACHE_WAYS 4
#define L2_CACHE_SETS 1024
#define L2_CACHE_SETS_LOG 10
#define L2_CACHE_SIZE (L2_CACHE_SETS) * (L2_CACHE_WAYS) * (CACHE_BLOCK_SIZE)

#define LLC_CACHE_WAYS 12
#define LLC_CACHE_SETS_TOTAL 16384
#define LLC_CACHE_SETS_PER_SLICE 2048
#define LLC_CACHE_SETS_LOG 11
#define LLC_CACHE_SLICES 8
#define LLC_CACHE_SIZE (LLC_CACHE_SETS_TOTAL) * (LLC_CACHE_WAYS) * (CACHE_BLOCK_SIZE)

/* 
 * Set indexes 
 */

#define L1_SET_INDEX_MASK 0xFC0				 /* 6 bits - [11-6] - 64 sets + 8 way for each core */
#define L2_SET_INDEX_MASK 0xFFC0			 /* 10 bits - [15-6] - 1024 sets + 4 way for each core  */
#define LLC_SET_INDEX_PER_SLICE_MASK 0x1FFC0 /* 11 bits - [16-6] - 2048 sets + 16 way for each slice  */
#define LLC_INDEX_STRIDE 0x20000			 /* Offset required to get the next address with the same LLC cache set index. 17 = bit 16 (MSB bit of LLC_SET_INDEX_PER_SLICE_MASK) + 1 */
#define L2_INDEX_STRIDE 0x10000				 /* Offset required to get the next address with the same L2 cache set index. 16 = bit 15 (MSB bit of L2_SET_INDEX_MASK) + 1 */

/*
 * Memory related constants
 * 
 * A page frame shifted left by PAGE_SHIFT will give us the physcial address of the frame
 * Note that this number is architecture dependent. For x86_64 it is defined as 12.
 */

#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8
#define PAGE 4096

// List related functions

struct Node {
	void *address;
	struct Node *next;
};

int list_length(struct Node *head);

void list_append(struct Node **head, void *addr);

void *list_pop(struct Node **head);

void list_push(struct Node **head, void *addr);

void list_concat(struct Node **ptr, struct Node *chunk);

void list_from_chunks(struct Node **ptr, struct Node **chunks, int avoid, int len);

void list_split(struct Node *ptr, struct Node **chunks, int n);

void shuffle(int *array, size_t n);

// Cache related functions

uint64_t get_cache_set_index(uint64_t addr, int cache_level);

inline void maccess(void *p)
{
	asm volatile("movq (%0), %%rax\n"
				 :
				 : "c"(p)
				 : "rax");
}

inline static uint32_t time_access(void *addr)
{
	uint32_t timing;

	asm volatile(
		"mov %1, %%rsi\n"
		"lfence\n"
		"rdtsc\n"
		"lfence\n"
		"mov %%eax, %%edi\n" /* store low order bits of tsc into edi */
		"movq (%%rsi), %%rsi\n" /* LOAD */
		"rdtscp\n"			 /* RDTSC */
		"lfence\n"
		"sub %%edi, %%eax\n" /* get timing difference */
		: "=a"(timing)		 /*output*/
		: "r"(addr)
		: "rcx", "rdx", "edi", "rsi", "memory");

	return timing;
}

inline static uint32_t time_access0(void *addr)
{
	uint32_t timing;

	asm volatile(
		"lfence\n"
		"rdtsc\n"
		"lfence\n"
		"mov %%eax, %%edi\n" /* store low order bits of tsc into edi */
		"rdtscp\n"			 /* RDTSC */
		"lfence\n"
		"sub %%edi, %%eax\n" /* get timing difference */
		: "=a"(timing)		 /*output*/
		: "r"(addr)
		: "rcx", "rdx", "edi", "rsi", "memory");

	return timing;
}

// Other utils

static void pin_cpu(size_t core_ID)
{
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(core_ID, &set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &set) < 0) {
		printf("Unable to Set Affinity\n");
		exit(EXIT_FAILURE);
	}
}

#endif
