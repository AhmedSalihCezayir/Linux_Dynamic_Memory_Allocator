#ifndef DMA_H
#define DMA_H

// Variables
int* p;
int* seg_start;

int power_m;
int segment_size;
int frag_size;

// Functions
int dma_init (int m);
void *dma_alloc (int size);
void dma_free (void *p);
void dma_print_page(int pno);
void dma_print_bitmap();
void dma_print_blocks();
int dma_give_intfrag();

int one_count();
#endif