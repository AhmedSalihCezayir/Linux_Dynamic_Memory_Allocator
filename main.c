#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(){
    dma_init(14);
    
    int* point;
    point = dma_alloc(8);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    //dma_free(point);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    //dma_print_bitmap();

    *point = 25;
    // printf("******************************\n");
    
    // dma_print_blocks();
    // printf("%p\n", p);

    dma_print_page(0);

    return 0;
}