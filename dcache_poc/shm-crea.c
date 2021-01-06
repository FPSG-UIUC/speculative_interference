// compile with: gcc -O3 shm-crea.c -o crea
// to run this you must be root, or in the group specified in /proc/sys/vm/hugetlb_shm_group 

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define HUGE_PGSIZE	(2*1024*1024)
#define NUM_PAGES 200
#define KEY 0xDEADB000

#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8

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
   size_t read_num = fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);
   return page_frame_number;
}

void check_contiguity(void *buffer, int id) {
   unsigned int page_frame_number = get_page_frame_number_of_address(buffer);
   unsigned int old_page_frame = page_frame_number;
   unsigned int pageoff = 0x1000;
   while(pageoff < HUGE_PGSIZE) {
     page_frame_number = get_page_frame_number_of_address(buffer+pageoff);
     //printf("Page frame: 0x%x\n", page_frame_number);
     if(page_frame_number - old_page_frame != 1) {
         printf("[!] Error[2]: mmaped buffer not contiguous going from frame: 0x%x to frame: 0x%x diff: 0x%x offset: 0x%x\n", old_page_frame, page_frame_number, old_page_frame - page_frame_number, pageoff);
     }
     pageoff += 0x1000;
     old_page_frame = page_frame_number;
   }
   printf("[+] Allocated contiguous buffer starting at physical frame: 0x%x, id: %d\n", (unsigned int)get_page_frame_number_of_address(buffer), id);
   return;
}

int main() {
    FILE *fids = fopen("./pageids.txt", "w+");
    if (fids == NULL) {
        printf("fopen failed, errno = %d\n", errno);
    }

    for(int i = 0; i < NUM_PAGES; i++) {
        char *page;
        int id;

        id = shmget(KEY+i, HUGE_PGSIZE, IPC_CREAT | IPC_EXCL | SHM_HUGETLB | 0644);
        if (id < 0) {
            perror("shmget");
            return 1;
        }

        //printf("%d\n", id);
        fprintf(fids, "%d\n", id);
        page = shmat(id, NULL, 0);
        if (page == MAP_FAILED) {
            perror("shmat");
            return 1;
        }
        shmctl(id, IPC_RMID, NULL);
        
        memset(page, 0, HUGE_PGSIZE);
        unsigned int page_frame_number = get_page_frame_number_of_address(page);
        //printf("shmem ready on id %d! %p %x\n", id, page, page_frame_number);
        if(mlock(page, HUGE_PGSIZE) == -1) {
            printf("Failed to lock page in memory:\n");
        }
        check_contiguity(page, id);
    }
    fclose(fids);
    
    while(1){}
    return 0;
}
