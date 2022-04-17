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

    seg_start = ((int*)p);

    if ((long long) p == -1) { // If mmap fails
        return -1;
    }

    else {
        // Initilize heap and fill with 1 to show that it's empty, expect first 2 word bcs they are start words of bitmap
        int heap_word_count = (int) pow(2, m-3);
        segment_size = heap_word_count;
        ((int*)p)[0] = 0;
        ((int*)p)[1] = 1;

        for(int i = 2; i < heap_word_count; i++){
            ((int*)p)[i] = 1;
        }

        // Fill portion of bitmap which represent itself + reserved area
        int bitmap_self_word_count = (int) pow(2, m-9);

        for(int i = 2; i < bitmap_self_word_count; i++){
            ((int*)p)[i] = 0;
        }

        ((int*)p)[bitmap_self_word_count] = 0;
        ((int*)p)[bitmap_self_word_count + 1] = 1;

        for(int i = bitmap_self_word_count + 2; i < bitmap_self_word_count + 32; i++){
            ((int*)p)[i] = 0;
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
        if( ((int*)p)[i] == 1 && ((int*)p)[i+1] == 1 && one_count( ((int*)p) + i) >= (hexa_size / 8) ) {
            ((int*)p)[i] = 0;
            ((int*)p)[i+1] = 1;
            int start_point = i + 2;

            for(int k = start_point; (hexa_size / 8) - 2 > 0 ; k++) { 
                ((int*)p)[k] = 0;
                hexa_size = hexa_size - 8;
            }
            return (void*) (p + (8 * i)); 
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

    if( ((int*)p)[i] == 0 && ((int*)p)[i+1] == 1){
        do{
            ((int*)p)[i] = 1;
            ((int*)p)[i + 1] = 1;
            i = i + 2;
        }
        while (!( ((int*)p)[i] == 1 && ((int*)p)[i+1] == 1) && !( ((int*)p)[i] == 0 && ((int*)p)[i+1] == 1) );
    }
}

void dma_print_page(int pno){
    // unsigned long long start_pos = ((unsigned long long int)p) + pno * (int) pow(2, 12);
    // unsigned long long end_pos = start_pos + (unsigned long long) pow(2, 12);
    // int count = 0;
    // char string[4]; 
    
    // for(unsigned long long i = (start_pos - (long long int) seg_start) / 8 ; i < segment_size && i < (end_pos - (long long int) seg_start) / 8; i++){
    //     if(count % 64 == 0){
    //         printf("\n");
    //     }

    //     string[i % 4] = seg_start[i];
        
    //     if(count % 4 == 0){
    //         printf("%s\n", string);
    //         cur = NULL;
    //     }
    //     count++;
    // }
}

void dma_print_bitmap(){
    for(int i = 0; i < (int) pow(2, power_m-6); i++){
        if( i != 0 && i % 8 == 0){
            printf(" ");
            if(i % 64 == 0){
                printf("\n");
            }
        }
        printf("%d", ((int*)p)[i]);
    }
    printf("\n");
}

void dma_print_blocks(){
    printf("p: %lx\n", (long) seg_start);
    int size;
    unsigned long long address;

    for(int i = 0; i < segment_size;){
        if(seg_start[i] == 1 && seg_start[i+1] == 1){
            int j;
            for(j = i + 2; seg_start[j] != 0; j++){
            }
            size = (j - i) * 8;
            address = (unsigned long long) (p + (8 * i));
            printf("F, 0x%016llx, 0x%x, (%d)\n", address, size, size);
            i = j;
        }
        else if(seg_start[i] == 0 && seg_start[i+1] == 1){
            int j;
            for(j = i + 2; seg_start[j] == 0 && seg_start[j+1] == 0; j = j + 2){
            }
            size = (j - i) * 8;
            address = (unsigned long long) (p + (8 * i));
            printf("A, 0x%016llx, 0x%x, (%d)\n", address, size, size);
            i = j;
        }
    }
}

int dma_give_intfrag(){
    return frag_size;
}


