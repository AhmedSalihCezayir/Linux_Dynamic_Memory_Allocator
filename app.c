#include "dma.h"
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

double calculateTime(){
    struct timeval currentTime;
    double curTime;
    gettimeofday(&currentTime, NULL);
    curTime = (currentTime.tv_usec * 0.001 + currentTime.tv_sec * 1000);
    return curTime;
}

int main(){

    dma_init(18); 

//***************FROM LOW TO HIGH************
//*------------------------------------------

    printf("\nFROM LOW TO HIGH \n----------\n");
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

//*************FROM HIGH TO LOW***************
//*------------------------------------------

//---------- ALLOC tests ----------

    printf("\nFROM HIGH TO LOW\n--------\n");
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

//******************************
//******FRAGMENTATION PART******
//******************************  
    printf("**********\n FRAGMENTATION PART \n************\n ");

    dma_init(18);

//--------- CHOSEN INPUTS------------
    printf("\nCHOSEN INPUTS\n-----------\n ");

    int before500 = dma_give_intfrag();
    void* point500 = dma_alloc(500);
    int after500 = dma_give_intfrag();

    int beforek = dma_give_intfrag();
    void* pointk = dma_alloc(1000);
    int afterk = dma_give_intfrag();

    int before25k = dma_give_intfrag();
    void* point25k = dma_alloc(2500);
    int after25k = dma_give_intfrag();

    int before5k = dma_give_intfrag();
    void* point5k = dma_alloc(5000);
    int after5k = dma_give_intfrag();
    
    int before10k = dma_give_intfrag();
    void* point10k = dma_alloc(10000);
    int after10k = dma_give_intfrag();

    int before15k = dma_give_intfrag();
    void* point15k = dma_alloc(15000);
    int after15k = dma_give_intfrag();

    printf("fragmentation amount for 500 byte allocation/total fragmentation so far: %d, %d \n", after500 - before500, after500);
    printf("fragmentation amount for 1000 byte allocation/total fragmentation so far: %d, %d \n", afterk - beforek, afterk);
    printf("fragmentation amount for 2500 byte allocation/total fragmentation so far: %d, %d \n", after25k - before25k, after25k);
    printf("fragmentation amount for 5000 byte allocation/total fragmentation so far: %d, %d \n", after5k - before5k, after5k);
    printf("fragmentation amount for 10000 byte allocation/total fragmentation so far: %d, %d \n", after10k - before10k, after10k);
    printf("fragmentation amount for 15000 byte allocation/total fragmentation so far: %d, %d \n", after15k - before15k, after15k);

    dma_print_bitmap();
    dma_print_blocks();
    dma_print_page(0);

    dma_free(point500);
    dma_free(pointk);
    dma_free(point25k);
    dma_free(point5k);
    dma_free(point10k);
    dma_free(point15k);


//------------ RANDOM INPUTS------------------
    printf("\nRANDOM INPUTS\n-----------\n ");

    srand(time(NULL));
    for(int i = 0; i < 15; i++){
        int random = rand() % 3001;
        dma_alloc(random);
        printf("allocated byte this run: %d, current fragmentation total: %d\n", random, dma_give_intfrag());
    }
    return 0;
}