#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int dma_init (int m){
    if (m < 14 || m > 22) {  // if m input is wrong
         return -1;
    }

    int size = (int) pow(2, m); // Calculate necessary size for bitmap
    p = mmap (NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // Returns start adress of segment

    if (*p == -1) { // If mmap fails
        return -1;
    }

    else {
        // Initilize heap and fill with 1 to show that it's empty, expect first 2 word bcs they are start words of bitmap
        int heap_word_count = (int) pow(2, m-3);
        p[0] = 0;
        p[1] = 1;

        for(int i = 2; i < heap_word_count; i++){
            p[i] = 1;
        }

        // Fill portion of bitmap which represent itself
        int bitmap_self_word_count = (int) pow(2, m-9);

        for(int i = 2; i < bitmap_self_word_count; i++){
            p[i] = 0;
        }
        return 0;
    }
}

// void *dma_alloc (int size){

// }

// void  dma_free (void *p);

// void  dma_print_page(int pno);

// void  dma_print_bitmap(){
    
// };

// void  dma_print_blocks();

// int   dma_give_intfrag();
