# DCU POC

This will be a guide on how to run the dcache_poc.

## Step 1: Observing Contention Effects

As a preliminary step we will take a look at the observed contention effects of the given construction used by dcache_poc on the current machine being tested.

### Prerequisites

Before we begin run the initial script as before:

```
sudo run-first.sh
```

### Generating Contention Histograms

The objective of this step is to observe the extent of contention caused by the VSQRTPD dependency chain used in the POC as well as the noise of the current running environment.

We will be working in the **./contention** directory.

Follow these steps to generate the histogram (depicted in Figure 9 of the paper) that compares the execution time of a load that is data-dependent on a VSQRTPD dependency chain in the presence of immediate subsequent contending instructions (independent ready-to-fire VSQRTPD instructions) and absence of contention (NOPs).

Note: Before compiling the testbench, the fields **HT1** and **HT2** need to be defined in contention.c. **HT1** is the cpuid of the clock thread used for timing, while **HT2** is the cpuid of the main thread. These need to be set to cpuids of two sibling hyperthreads. To find sibling hyperthreads for a given cpuid (cpu0 for example) one can execute the following:

```
cat /sys/devices/system/cpu/cpu0/topology/thread_siblings_list
```

If the current machine does not support SMT, one can also run under different cores, however the timing measured by the clock_thread might have added noise.

Note: There is a configurable parameter in contention.c which determines the length of subsequent contending instructions. This is the value following **.REPT** commented as **"CON_LOOPS"**. By default this is set to 1, however we will also run the test using 10 iterations.

Once **HT1**, **HT2**, and **CON_LOOPS** are set, to generate the histogram run:

```
cd ./contention
gcc -std=gnu99 -O3 contention.c -lpthread -o con 
./con > raw.csv
python plotContention.py
```

Subsequent runs with updates to **contention.c** can be done simply using:

```
gcc -std=gnu99 -O3 contention.c -lpthread -o con && rm raw.csv && ./con > raw.csv && python plotContention.py
```

Below we have histograms showing sufficient contention to run the dcache_poc on our machine. 

For CON_LOOPS == 1:
![alt text](./figures/loop1.png)

For CON_LOOPS == 10:
![alt text](./figures/loop10.png)

Note: The X-axis is clock-thread cycles, to convert to rdtsc cycles one can time using rdtsc a number of clock-thread ticks and determine the conversion ratio. For our tests there is approx 4.92 rdtsc cycles per clock-thread tick.

If the contention histograms do not indicate a clean seperation in execution times, background processes need to terminated and preferably ubuntu be run in non-gui mode if not already. 

## Step 2: Preparing Shared Memory for dcache_poc

In this step we will allocate a contiguous set of physical memory pages for use by the dcache_poc to create eviction sets. 

### Step-by-Step

This process, **crea** will create **NUM_PAGES** contiguous huge pages and save the shared memory ids in a file: **page_ids.txt**. The dcache_poc will then reference this file to use in eviction set creation. Once the shared memory ids are ready, they will detach from **crea**, and the process should be suspended. This will allow dcache_poc to use these shared pages later, and once dcache_poc terminates and **crea** terminates the shared memory pages will be freed. To do this do the following:

```
gcc -O3 shm-crea.c -o crea
sudo ./crea
Ctrl+Z
```

Check any error messages and the pageids.txt file for **NUM_PAGES** (200) valid shared memory ids.


## Step 3: Running DCU POC

## License

## Acknowledgments


