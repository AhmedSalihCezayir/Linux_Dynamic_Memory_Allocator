#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    dma_init(14);

    for(int i = 0; i < pow(2,8); i++){
        printf("%d: %d\n", i, p[i]);
    }

    dma_alloc(32);
    dma_alloc(64);
    
    for(int i = 0; i < pow(2,8); i++){
        printf("%d: %d\n", i, p[i]);
    }

    return 0;
}