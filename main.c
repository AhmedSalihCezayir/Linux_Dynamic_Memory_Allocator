#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

double calculateTime(){
    struct timeval currentTime;
    double curTime;
    gettimeofday(&currentTime, NULL);
    curTime = (currentTime.tv_usec * 0.001 + currentTime.tv_sec * 1000);
    return curTime;
}

int main(){

    dma_init(18);

//*******************************************
//***************FROM LOW TO HIGH************
//*******************************************

    printf("\n************\n FROM LOW TO HIGH \n************\n");
    printf("\nALLOC TESTS\n");

//---------- ALLOC tests ----------

    void* point256;
    int* point2k;
    int* point16k;
    int* point32k;
    int* point65k;
    int* point131k;

    double before256 = calculateTime();
    point256 = dma_alloc(256);
    double after256 = calculateTime();

    double before2k = calculateTime();
    point2k = dma_alloc(2048);
    double after2k = calculateTime();

    double before16k = calculateTime();
    point16k = dma_alloc(16384);
    double after16k = calculateTime();

    double before32k = calculateTime();
    point32k = dma_alloc(32768);
    double after32k = calculateTime();

    double before65k = calculateTime();
    point65k = dma_alloc(65536);
    double after65k = calculateTime();

    double before131k = calculateTime();
    point131k = dma_alloc(131072);
    double after131k = calculateTime();

    printf("time needed for 256 byte allocation: %.5f \n", after256 - before256);
    printf("time needed for 2048 byte allocation: %.5f \n", after2k - before2k);
    printf("time needed for 16384 byte allocation: %.5f \n", after16k - before16k);
    printf("time needed for 32768 byte allocation: %.5f \n", after32k - before32k);
    printf("time needed for 65536 byte allocation: %.5f \n", after65k - before65k);
    printf("time needed for 131072 byte allocation: %.5f \n", after131k - before131k);

//---------- DEALLOC tests ----------

    printf("\nDEALLOC TESTS\n");
  
    before256 = calculateTime();
    dma_free(point256);
    after256 = calculateTime();

    before2k = calculateTime();
    dma_free(point2k);
    after2k = calculateTime();

    before16k = calculateTime();
    dma_free(point16k);
    after16k = calculateTime();

    before32k = calculateTime();
    dma_free(point32k);
    after32k = calculateTime();

    before65k = calculateTime();
    dma_free(point65k);
    after65k = calculateTime();

    before131k = calculateTime();
    dma_free(point131k);
    after131k = calculateTime();

    printf("time needed for 256 byte deallocation: %.5f \n", after256 - before256);
    printf("time needed for 2048 byte deallocation: %.5f \n", after2k - before2k);
    printf("time needed for 16384 byte deallocation: %.5f \n", after16k - before16k);
    printf("time needed for 32768 byte deallocation: %.5f \n", after32k - before32k);
    printf("time needed for 65536 byte deallocation: %.5f \n", after65k - before65k);
    printf("time needed for 131072 byte deallocation: %.5f \n", after131k - before131k);

//********************************************
//*************FROM HIGH TO LOW***************
//********************************************

//---------- ALLOC tests ----------

    printf("\n************\n FROM HIGH TO LOW \n************\n");
    printf("\nALLOC TESTS\n");

    before131k = calculateTime();
    point131k = dma_alloc(131072);
    after131k = calculateTime();

    before65k = calculateTime();
    point65k = dma_alloc(65536);
    after65k = calculateTime();

    before32k = calculateTime();
    point32k = dma_alloc(32768);
    after32k = calculateTime();

    before16k = calculateTime();
    point16k = dma_alloc(16384);
    after16k = calculateTime();

    before2k = calculateTime();
    point2k = dma_alloc(2048);
    after2k = calculateTime();

    before256 = calculateTime();
    point256 = dma_alloc(256);
    after256 = calculateTime();

    printf("time needed for 256 byte allocation: %.5f \n", after256 - before256);
    printf("time needed for 2048 byte allocation: %.5f \n", after2k - before2k);
    printf("time needed for 16384 byte allocation: %.5f \n", after16k - before16k);
    printf("time needed for 32768 byte allocation: %.5f \n", after32k - before32k);
    printf("time needed for 65536 byte allocation: %.5f \n", after65k - before65k);
    printf("time needed for 131072 byte allocation: %.5f \n", after131k - before131k);
    
//---------- DEALLOC tests ----------
    
    printf("\nDEALLOC TESTS\n");

    before131k = calculateTime();
    dma_free(point131k);
    after131k = calculateTime();

    before65k = calculateTime();
    dma_free(point65k);
    after65k = calculateTime();

    before32k = calculateTime();
    dma_free(point32k);
    after32k = calculateTime();

    before16k = calculateTime();
    dma_free(point16k);
    after16k = calculateTime();

    before2k = calculateTime();
    dma_free(point2k);
    after2k = calculateTime();

    before256 = calculateTime();
    dma_free(point256);
    after256 = calculateTime();

    printf("time needed for 256 byte deallocation: %.15f \n", after256 - before256);
    printf("time needed for 2048 byte deallocation: %.15f \n", after2k - before2k);
    printf("time needed for 16384 byte deallocation: %.5f \n", after16k - before16k);
    printf("time needed for 32768 byte deallocation: %.5f \n", after32k - before32k);
    printf("time needed for 65536 byte deallocation: %.5f \n", after65k - before65k);
    printf("time needed for 131072 byte deallocation: %.5f \n", after131k - before131k);

//-----------------------------------------------

    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    // dma_free(point);
    
    // for(int i = 0; i < pow(2,8); i++){
    //     printf("%d: %d\n", i, p[i]);
    // }

    //dma_print_bitmap();
    
    // dma_print_blocks();
    // printf("%p\n", p);

    // dma_print_page(0);

    return 0;
}