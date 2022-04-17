#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int dma_init (int m){
    power_m = m;

    if (m < 14 || m > 22) {  // if m input is wrong
         return -1;
    }

    int size = (int) pow(2, m); // Calculate necessary size for bitmap
    p = mmap (NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // Returns start adress of segment

    seg_start = p;

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
        frag_size += 16 - (size % 16); // Fragmentation size updated
        int div = size / 16;
        hexa_size = (div + 1) * 16;
    }

    for(int i = 0; i < segment_size; i = i + 2){
        if(p[i] == 1 && p[i+1] == 1 && one_count(p + i) >= (hexa_size / 8) ) {
            p[i] = 0;
            p[i+1] = 1;
            int start_point = i + 2;

            for(int k = start_point; (hexa_size / 8) - 2 > 0 ; k++) { 
                p[k] = 0;
                hexa_size = hexa_size - 8;
            }
            return p + (8 * i); 
        }
    }
    return NULL;
}

// dma_free helper function
int one_count(int* p){
    int i = 0;
    int count = 0;
    while(p[i] == 1){
        count++;
        i++;
    }
    return count;
}

void dma_free (void *target_loc){
    int i = ( (int*) target_loc - seg_start) / 8;

    if(p[i] == 0 && p[i+1] == 1){
        do{
            p[i] = 1;
            p[i + 1] = 1;
            i = i + 2;
        }
        while (!(p[i] == 1 && p[i+1] == 1) && !(p[i] == 0 && p[i+1] == 1) );
    }
}

// void dma_print_page(int pno){}

void dma_print_bitmap(){
    for(int i = 0; i < (int) pow(2, power_m-6); i++){
        if( i != 0 && i % 8 == 0){
            printf(" ");
            if(i % 64 == 0){
                printf("\n");
            }
        }
        printf("%d",p[i]);
    }
    printf("\n");
}

void dma_print_blocks(){
    int size;
    int address;

    for(int i = 0; i < segment_size;){
        if(seg_start[i] == 1 && seg_start[i+1] == 1){
            int j;
            for(j = i + 2; seg_start[j] != 0; j++){
            }
            size = (j - i) * 8;
            address = p + (8 * i);
            printf("F, 0x%016x, 0x%x, (%d)\n", address, size, size);
            i = j;
        }
        else if(seg_start[i] == 0 && seg_start[i+1] == 1){
            int j;
            for(j = i + 2; seg_start[j] == 0 && seg_start[j+1] == 0; j = j + 2){
            }
            size = (j - i) * 8;
            address = p + (8 * i);
            printf("A, 0x%016x, 0x%x, (%d)\n", address, size, size);
            i = j;
        }
    }
}

int dma_give_intfrag(){
    return frag_size;
}
