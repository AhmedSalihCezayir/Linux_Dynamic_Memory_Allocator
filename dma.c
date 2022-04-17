#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int dma_init (int m){
    pthread_mutex_init(&p_lock, NULL);
    pthread_mutex_init(&power_m_lock, NULL);
    pthread_mutex_init(&segment_size_lock, NULL);
    pthread_mutex_init(&frag_size_lock, NULL);

    pthread_mutex_lock(&power_m_lock);
    frag_size = 0;
    power_m = m;
    pthread_mutex_unlock(&power_m_lock);
    
    if (m < 14 || m > 22) {  // if m input is wrong
         return -1;
    }

    int size = (int) pow(2, m); // Calculate necessary size for bitmap

    pthread_mutex_lock(&p_lock);
    p = mmap (NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // Returns start adress of segment
    
    seg_start = ((int*)p);
    pthread_mutex_unlock(&p_lock);

    if ((long long) p == -1) { // If mmap fails
        return -1;
    }

    else {
        // Initilize heap and fill with 1 to show that it's empty, expect first 2 word bcs they are start words of bitmap
        int heap_word_count = (int) pow(2, m-3);

        pthread_mutex_lock(&segment_size_lock);
        segment_size = heap_word_count;
        pthread_mutex_unlock(&segment_size_lock);

        pthread_mutex_lock(&p_lock);
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
        pthread_mutex_unlock(&p_lock);
        return 0;
    }
}

void *dma_alloc (int size){
    int hexa_size = size;
    void* allocated = NULL;

    if(size % 16 != 0){ // Size will be equalized to upper multiple of 16
        pthread_mutex_lock(&frag_size_lock);
        frag_size += 16 - (size % 16); // Fragmentation size updated
        pthread_mutex_unlock(&frag_size_lock);

        int div = size / 16;
        hexa_size = (div + 1) * 16;
    }

    for(int i = 0; i < segment_size; i = i + 2){
        pthread_mutex_lock(&p_lock);
        if( ((int*)p)[i] == 1 && ((int*)p)[i+1] == 1 && one_count( ((int*)p) + i) >= (hexa_size / 8) ) {
            ((int*)p)[i] = 0;
            ((int*)p)[i+1] = 1;
            int start_point = i + 2;

            for(int k = start_point; (hexa_size / 8) - 2 > 0 ; k++) { 
                ((int*)p)[k] = 0;
                hexa_size = hexa_size - 8;
            }
            allocated = (void*) ((int*)p + (8 * i));
            break; 
        }
        pthread_mutex_unlock(&p_lock);
    }
    return allocated;
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
    int i = ( (int*) target_loc - (int*) p) / 8;

    if(i > segment_size){
        printf("MEMORY ACCESS VIOLATION !!!!!!\n");
    }
    else if(seg_start[i] == 0 && seg_start[i+1] == 1) {
        do{
            seg_start[i] = 1;
            seg_start[i + 1] = 1;
            i = i + 2;
        }
        while (!(seg_start[i] == 1 && seg_start[i+1] == 1) && !(seg_start[i] == 0 && seg_start[i+1] == 1) );
    }
}

void dma_print_page(int pno){
    unsigned long long start_pos = ((unsigned long long int)p) + pno * (int) pow(2, 12);
    pthread_mutex_lock(&p_lock);
    for(unsigned long long i = (start_pos - (long long int) seg_start) / 8 ; i * 8 < segment_size; i = i + 4){
        if(i % 256 == 0){
            printf("\n");
        }
        char binaryString[4];
        for (int k = 0; k < 4; k++) {
            char chr = ((int*) p)[i + k] + '0';
            binaryString[k] = chr;
        }

        // Convert binary string to integer
        int value = (int)strtol(binaryString, NULL, 2);

        // Print the integer as hexadecimal
        printf("%x", value);    
    }
    pthread_mutex_unlock(&p_lock);
}

void dma_print_bitmap(){
    for(int i = 0; i < (int) pow(2, power_m-6); i++){
        if( i != 0 && i % 8 == 0){
            printf(" ");
            if(i % 64 == 0){
                printf("\n");
            }
        }
        pthread_mutex_lock(&p_lock);
        printf("%d", ((int*)p)[i]);
        pthread_mutex_unlock(&p_lock);
    }
    printf("\n");
}

void dma_print_blocks(){
    int size;
    unsigned long long address;

    pthread_mutex_lock(&p_lock);
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
    pthread_mutex_unlock(&p_lock);
}

int dma_give_intfrag(){
    return frag_size;
}


