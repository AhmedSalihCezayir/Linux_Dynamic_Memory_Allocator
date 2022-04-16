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
        segment_size = heap_word_count;
        p[0] = 0;
        p[1] = 1;

        for(int i = 2; i < heap_word_count; i++){
            p[i] = 1;
        }

        // Fill portion of bitmap which represent itself + reserved area
        int bitmap_self_word_count = (int) pow(2, m-9);

        for(int i = 2; i < bitmap_self_word_count; i++){
            p[i] = 0;
        }

        p[bitmap_self_word_count] = 0;
        p[bitmap_self_word_count + 1] = 1;

        for(int i = bitmap_self_word_count + 2; i < bitmap_self_word_count + 32; i++){
            p[i] = 0;
        }

        return 0;
    }
}

void *dma_alloc (int size){
    int hexa_size = size;

    if(size % 16 != 0){ // Size will be equalized to upper multiple of 16
        int div = size / 16;
        hexa_size = (div + 1) * 16;
    }

    for(int i = 0; i < segment_size; i = i + 2){
        if(p[i] == 1 && p[i+1] == 1 && one_count(p + i) >= (hexa_size / 8) ){
            printf("*** start of if: %d \n", i);
            p[i] = 0;
            p[i+1] = 1;
            printf("*** p[i]: %d \n", p[i]);
            int start_point = i + 2;

            for(int k = start_point; (hexa_size / 8) - 2 > 0 ; k++){ // ! In case of problem with allocation, this might be doing odd numbered iterations
                p[k] = 0;
                hexa_size = hexa_size - 8;
            }
            
            return p + i;
        }
    }
    return NULL;
}

int one_count(int* p){
    int i = 0;
    int count = 0;
    while(p[i] == 1){
        count++;
        i++;
    }
    return count;
}

// void  dma_free (void *p);

// void  dma_print_page(int pno);

// void  dma_print_bitmap(){
    
// };

// void  dma_print_blocks();

// int   dma_give_intfrag();
