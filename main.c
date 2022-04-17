#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(){
    dma_init(18);
    
    void* p1 = dma_alloc(256);
    void* p2 = dma_alloc(24);
    void* p3 = dma_alloc(7);

    dma_print_blocks();
    dma_print_bitmap();
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    dma_free(p1);
    dma_free(p2);
    dma_free(p3);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    //dma_print_bitmap();

    // printf("******************************\n");
    
    // dma_print_blocks();
    // printf("%p\n", p);

    dma_print_blocks();
    dma_print_bitmap();
    return 0;
}