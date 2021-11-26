FILE1 = basicmath_small.c   rad2deg.c  cubic.c   isqrt.c
FILE2 = basicmath_large.c   rad2deg.c  cubic.c   isqrt.c
CROSS_COMPILE = /opt/alphaev67-unknown-linux-gnu/bin/alphaev67-unknown-linux-gnu
CC=$(CROSS_COMPILE)-gcc

all: basicmath_large

basicmath_large: ${FILE2} Makefile
	$(CC) -static -O3 ${FILE2} -o basicmath_large -lm

clean:
	rm -rf basicmath_large output*
