all: libdma.a  main

libdma.a: dma.c
	gcc -Wall -g -c dma.c
	ar rcs libdma.a dma.o 

main: main.c
	gcc -Wall -g -o main main.c -L. -ldma -lm

clean:
	rm -fr *~ libdma.a dma.o main.o main