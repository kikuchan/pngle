CFLAGS+=-Os -std=c99
CFLAGS += -I../include
CFLAGS += -Wall -Wextra -Wno-misleading-indentation -Wno-unknown-warning-option
# CFLAGS += -DPNGLE_NO_GAMMA_CORRECTION

PNGLE_C=../src/pngle.c ../src/miniz.c
PNGLE_H=../include/pngle.h ../include/miniz.h

all: test

pngle-png2ppm: ../examples/pngle-png2ppm.c $(PNGLE_C) $(PNGLE_H) Makefile
	$(CC) $(CFLAGS) -o pngle-png2ppm ../examples/pngle-png2ppm.c $(PNGLE_C) -lm

pngle-info: ../examples/pngle-info.c $(PNGLE_C) $(PNGLE_H) Makefile
	$(CC) $(CFLAGS) -o pngle-info ../examples/pngle-info.c $(PNGLE_C) -lm

test: pngle-png2ppm pngle-info
	sh test.sh

clean:
	rm -f pngle-png2ppm pngle-info
