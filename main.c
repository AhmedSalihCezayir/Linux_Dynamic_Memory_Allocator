#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    dma_init(14);
    
    void* point;
    point = dma_alloc(32);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    printf("******************************\n");

    //dma_free(point);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    dma_print_bitmap();

    return 0;
}