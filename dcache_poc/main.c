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
 
  /*while(ptr != NULL) {
    maccess(ptr->address);
    maccess(ptr->address);
    ptr = ptr->next;
  }*/
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



static int test_eviction_setAB(struct Node *addr_set, void *addrA, void *addrB)
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

	for (i = 0; i < TEST_REPETITIONS; i++) {
    
 
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
		maccess(addrB);
    
 /*    
		maccess(addrA);
   
		maccess(addrA);
		maccess(addrB);
   
		maccess(addrA);
    
    maccess(addr12);
    maccess(addr13);
    maccess(addr14);
    maccess(addr15);
    maccess(addr16);
    maccess(addr17);
    maccess(addr18);
    maccess(addr19);
    maccess(addr20);
    maccess(addr21);
    maccess(addr22);
    */
    maccess(addrA + 222); // page walk
		latency2 = time_access(addrA);
   
    maccess(addrB + 222); // page walk
		latency1 = time_access(addrB);  


    maccess(addr12);
    maccess(addr13);
    maccess(addr14);
    maccess(addr15);
    maccess(addr16);
    maccess(addr17);
    maccess(addr18);
    maccess(addr19);
    maccess(addr20);
    maccess(addr21);
    maccess(addr22);

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

static int test_eviction_setBA(struct Node *addr_set, void *addrA, void *addrB)
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

	for (i = 0; i < TEST_REPETITIONS; i++) {
    
 
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
    
	  maccess(addrB);
		maccess(addrA);
    
 /*    
		maccess(addrA);
   
		maccess(addrA);
		maccess(addrB);
   
		maccess(addrA);
    
    maccess(addr12);
    maccess(addr13);
    maccess(addr14);
    maccess(addr15);
    maccess(addr16);
    maccess(addr17);
    maccess(addr18);
    maccess(addr19);
    maccess(addr20);
    maccess(addr21);
    maccess(addr22);
    */
    maccess(addrA + 222); // page walk
		latency2 = time_access(addrA);
   
    maccess(addrB + 222); // page walk
		latency1 = time_access(addrB);  


    maccess(addr12);
    maccess(addr13);
    maccess(addr14);
    maccess(addr15);
    maccess(addr16);
    maccess(addr17);
    maccess(addr18);
    maccess(addr19);
    maccess(addr20);
    maccess(addr21);
    maccess(addr22);

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
  list_append(&eviction_set, (void *)0x7ffff1480000);
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
  list_append(&eviction_set, (void *)0x7fffe1480000);
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
  printf("[+] Picked victim address %p (cache set = %d)\n", victim_addr, cache_set);
  
  cache_miss_treshold = calibrate_cache_miss_threshold (victim_addr);
  printf("[+] Calibrated Cache Miss Threshold = %d\n", cache_miss_treshold);
  
  
  while(found == 0) {
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
    	if(debug) printf("[+] Initial candidate set evicted victim\n");
    } else {
    	if(debug) printf("[!] Error: invalid candidate set, try again\n");
      eviction_set = NULL;
      continue;
    	//exit(0);
    }
    
    // Reduce the eviction set
    if(debug) printf("[+] Starting reduction...\n");
    ret = reduce_eviction_set_gt(&eviction_set, victim_addr);
    len = list_length(eviction_set);
    if (ret) {
    	if(debug) printf("[!] Error: optimal eviction set not found or not working (length=%d)\n", len);
      eviction_set = NULL;
      continue;
    	//exit(0);
    } else {
    	if(debug) printf("[+] Found minimal eviction set for %p (length=%d)\n", victim_addr, len);
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
    if (successes > 15) {
    	printf("[+] Works %d / %d times\n", successes, i);
      found = 1;
    } else {
    	printf("[!] Works %d / %d times\n", successes, i);
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

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
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
int main(int argc, char *argv[])
{
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
/*	buffer = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);

 	if (buffer == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
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
  */
/*  FILE *fp = fopen("pageids.txt", "r");
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
    //shmctl(id, IPC_RMID, NULL);
    segbuffer[i] = page;
  }
  for(int i = 0; i < 200; i++) {
      //printf("%p\n", segbuffer[i]); 
  }
  printf("%p\n", segbuffer[199]); 
  buffer = segbuffer[199];*/
buffer = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  //printf("%x\n", get_page_frame_number_of_address((void *)0x7fffed8a0000));
  /*list_append(&eviction_set, (void *)0x7fffedb80000);
  list_append(&eviction_set, (void *)0x7ffff1240000);
  list_append(&eviction_set, (void *)0x7ffff1480000);
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
  struct Node* eviction_set1 = find_eviction_set(buffer, buffer, &len1);
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
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000); 
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000);
  test_flush_set(eviction_set1, buffer, 0x18000); 
  test_flush_set(eviction_set1, buffer, 0x18000); 
  printf("\n");
    printf("L1+L2 flush_set");
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000); 
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000);
  test_flush_set(eviction_set1, buffer, 0x10000); 
  test_flush_set(eviction_set1, buffer, 0x10000); 
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
   
  return;
   

  
  
  struct Node* eviction_set4 = find_eviction_set(buffer, buffer, &len1);
  //struct Node* eviction_set1 = static_eviction_set1(&len1);
  print_eviction_set(eviction_set1, len1);
  int successes1s = 0;
    for (i = 0; i < 20; i++) {
    	ret = test_eviction_set(eviction_set1, buffer);
    	if (ret) {
    		successes1s += 1;
    	}
    }
    	printf("[+] Works %d / %d times\n", successes1s, i);
     
  struct Node* eviction_set2 = find_eviction_set(buffer, eviction_set1->address, &len2);
  //struct Node* eviction_set2 = static_eviction_set2(&len2);
  print_eviction_set(eviction_set2, len2);
  int successes2s = 0;
    for (i = 0; i < 20; i++) {
    	ret = test_eviction_set(eviction_set2, eviction_set1->address);
    	if (ret) {
    		successes2s += 1;
    	}
    }
    	printf("[+] Works %d / %d times\n", successes2s, i);
        
  struct Node* eviction_set3 = find_eviction_set(buffer, eviction_set2->address, &len3);
  //struct Node* eviction_set3 = static_eviction_set3(&len3);
  print_eviction_set(eviction_set3, len3);
  
  
  void *addrA = buffer;
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
    printf("too many duplicates len = %d\n", final_len); 
    exit(1);
  }
  
  printf("[+] AddrA = %p\n", addrA);
  printf("[+] AddrB = %p\n", addrB);
  print_eviction_set(eviction_final, final_len);
  

  int successes1 = 0;
  for (i = 0; i < 20; i++) {
  	ret = test_eviction_set(eviction_final, addrA);
  	if (ret) {
  		successes1 += 1;
  	}
  }
  printf("[+] evset_final + addrA Works %d / %d times\n", successes1, i);
  
  int successes2 = 0;
  for (i = 0; i < 20; i++) {
  	ret = test_eviction_set(eviction_final, addrB);
  	if (ret) {
  		successes2 += 1;
  	}
  }
  printf("[+] evset_final + addrB Works %d / %d times\n", successes2, i);
  
  int successes3 = 0;
  for (i = 0; i < 20; i++) {
  	ret = test_eviction_set(eviction_set1, addrA);
  	if (ret) {
  		successes3 += 1;
  	}
  }
  printf("[+] evset1 + addrA Works %d / %d times\n", successes3, i);
  
  int successes4 = 0;
  for (i = 0; i < 20; i++) {
  	ret = test_eviction_set(eviction_set2, addrB);
  	if (ret) {
  		successes4 += 1;
  	}
  }
  printf("[+] evset2 + addrB Works %d / %d times\n", successes4, i);
  
  if(successes1 > 15 && successes2 > 15 && successes3 > 15 && successes4 > 15) {
    printf("GOOD SET\n");
    test_eviction_setAB(eviction_final, addrA, addrB);
    test_eviction_setBA(eviction_final, addrA, addrB);
    
  }
  return 0;
	// Prepare two threads, one for victim and one for attacker
	pthread_t victim_thread, attacker_thread;

	// Launch threads
	// TODO write functions
	// pthread_create(&victim_thread, NULL, victim_function, (void *) victim_input);
	// pthread_create(&attacker_thread, NULL, attacker_function, (void *) attacker_input);

	// Wait for the threads to complete
	pthread_join(victim_thread, NULL);
	pthread_join(attacker_thread, NULL);

}
