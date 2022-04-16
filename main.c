#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    dma_init(12);

    for(int i = 0; i < pow(2,7); i++){
        printf("%d: %d\n", i, p[i]);
    }

    return 0;
}