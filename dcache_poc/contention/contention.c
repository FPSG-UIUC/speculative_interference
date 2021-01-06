// compile with "gcc -std=gnu99 -O3 contention.c -lpthread -o con"
// outputs a CSV, one line for each test with all data points
// cat /sys/devices/system/cpu/cpu0/topology/thread_siblings_list

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#define HT1	0
#define HT2	6

#ifndef HT1
#error "edit source to define ids of sibling hyperthreads"
#endif

volatile unsigned int ticks;
unsigned int __attribute__ ((aligned (512))) array[8] = {0,1,2,3,4,5,6,7};
double __attribute__ ((aligned (512))) val_array[4] = {0.0,2.0,3.0,5.0};

void setaffinity(int cpu)
{
    cpu_set_t c;

    CPU_ZERO(&c);
    CPU_SET(cpu, &c);

    if (sched_setaffinity(0, sizeof(c), &c) < 0) {
        perror("sched_setaffinity");
        exit(1);
    }
}


int __attribute__((noinline)) baseline_test(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	double result;
	unsigned int rdtsc0;
	unsigned int rdtsc1;	
	
	unsigned int *arrptr = (unsigned int *)malloc(4*8*8);
	arrptr[0] = 0;
	arrptr[1] = 1;
	arrptr[2] = 2;
	arrptr[3] = 3;
	arrptr[4] = 4;
	arrptr[5] = 5;
	arrptr[6] = 6;
	arrptr[7] = 7;
	unsigned int *indicies = &array[0];

	double *valptr = (double *)malloc(8*4*8);
	valptr[0] = 7.0;
	valptr[1] = 2.0;
	valptr[2] = 2.0;
	valptr[3] = 8.0;
	double *values = &val_array[0];

	asm(
        R"(
            vzeroall
            vmovdqa (%2), %%ymm0
            vmovdqa (%3), %%ymm1
            vmovapd %%ymm1, %%ymm2
            vmovapd %%ymm1, %%ymm3
            vmovapd %%ymm1, %%ymm4
            vmovapd %%ymm1, %%ymm5
            vmovapd %%ymm1, %%ymm6
            vmovapd %%ymm1, %%ymm7
            vmovapd %%ymm1, %%ymm8
            vmovapd %%ymm1, %%ymm9
            vmovapd %%ymm1, %%ymm10
            vmovapd %%ymm1, %%ymm11
            vmovapd %%ymm1, %%ymm12
            vmovapd %%ymm1, %%ymm13
            vmovapd %%ymm1, %%ymm14
            vmovapd %%ymm1, %%ymm15

            lfence
            movl (%1), %0
            // Sample clock_ticks start value
            lfence

            .REPT   32
                vsqrtpd %%ymm1,%%ymm1
                //vpermd %%ymm1, %%ymm0, %%ymm1
            .ENDR

            vmovq %%xmm1, %%rax
            movl (%1,%%rax,1), %%r13d
            // Sample clock_ticks end value (data dependent on completion of vsqrtpd chain)

            .REPT	1024
                // Non-contending instructions
                NOP
            .ENDR

            sub %0, %%r13d
            movl %%r13d, %0
        )"
        : "=r"(cycle), "+r"(t), "+r"(indicies), "+r"(values), "=r"(result), "=r"(rdtsc0), "=r"(rdtsc1)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
    );
    
	//printf("cycle = %d, rdtsc = %d, ratio = %f\n", cycle, rdtsc1-rdtsc0, (double)(rdtsc1-rdtsc0)/cycle);
	return cycle;
}

int __attribute__((noinline)) contention_test(void)
{
	volatile unsigned int *t = &ticks;
	unsigned int cycle;
	double result;
	unsigned int rdtsc0;
	unsigned int rdtsc1;	
	
	unsigned int *arrptr = (unsigned int *)malloc(4*8*8);
	arrptr[0] = 0;
	arrptr[1] = 1;
	arrptr[2] = 2;
	arrptr[3] = 3;
	arrptr[4] = 4;
	arrptr[5] = 5;
	arrptr[6] = 6;
	arrptr[7] = 7;
	unsigned int *indicies = &array[0];

	double *valptr = (double *)malloc(8*4*8);
	valptr[0] = 7.0;
	valptr[1] = 2.0;
	valptr[2] = 2.0;
	valptr[3] = 8.0;
	double *values = &val_array[0];

	asm(
        R"(
            vzeroall
            vmovdqa (%2), %%ymm0
            vmovdqa (%3), %%ymm1
            vmovapd %%ymm1, %%ymm2
            vmovapd %%ymm1, %%ymm3
            vmovapd %%ymm1, %%ymm4
            vmovapd %%ymm1, %%ymm5
            vmovapd %%ymm1, %%ymm6
            vmovapd %%ymm1, %%ymm7
            vmovapd %%ymm1, %%ymm8
            vmovapd %%ymm1, %%ymm9
            vmovapd %%ymm1, %%ymm10
            vmovapd %%ymm1, %%ymm11
            vmovapd %%ymm1, %%ymm12
            vmovapd %%ymm1, %%ymm13
            vmovapd %%ymm1, %%ymm14
            vmovapd %%ymm1, %%ymm15

            lfence
            movl (%1), %0
            // Sample clock_ticks start value
            lfence

            .REPT   32
                vsqrtpd %%ymm1,%%ymm1
                //vpermd %%ymm1, %%ymm0, %%ymm1
            .ENDR

            vmovq %%xmm1, %%rax
            movl (%1,%%rax,1), %%r13d
            // Sample clock_ticks end value (data dependent on completion of vsqrtpd chain)

            // [CON_LOOPS]
            .REPT	1
                // Contending instructions
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
        
            sub %0, %%r13d
            movl %%r13d, %0
        )"
        : "=r"(cycle), "+r"(t), "+r"(indicies), "+r"(values), "=r"(result), "=r"(rdtsc0), "=r"(rdtsc1)
        : 
        : "%r12", "%r13", "%eax", "%ebx", "%ecx", "%edx", "%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7", "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"
    );
    
	//printf("cycle = %d, rdtsc = %d, ratio = %f\n", cycle, rdtsc1-rdtsc0, (double)(rdtsc1-rdtsc0)/cycle);
	return cycle;
}



void measure(char *msg, int (*f)(void))
{
	int i,n;

	//printf("%s", msg);
	printf("0");
	for(n=0;n<10000;n++) {
		i = f();
		printf(",%d", i);
	}

	printf("\n");
	fflush(stdout);
}

void* clock_thread(void *p)
{
	setaffinity(HT2);
	
	while (1) {
		ticks++;
	}

	return 0;
}

int main(void)
{
	pthread_t t;

	setaffinity(HT1);

	pthread_create(&t, NULL, clock_thread, NULL);
	while (ticks == 0) {}

	measure("baseline-1", baseline_test);
	measure("contention-1", contention_test);
	measure("contention-2", contention_test);
	measure("baseline-2", baseline_test);

	return 0;
}
