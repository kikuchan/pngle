CFLAGS += -Os -std=c99
CFLAGS += -g
CFLAGS += -I../src
CFLAGS += -Wall -Wextra -Wno-misleading-indentation -Wno-unknown-warning-option
# CFLAGS += -DPNGLE_NO_GAMMA_CORRECTION

EXAMPLES=../examples

all: test

.PHONY: pngle-png2ppm pngle-info
pngle-png2ppm:
	make -C $(EXAMPLES)/pngle-png2ppm
	cp $(EXAMPLES)/pngle-png2ppm/pngle-png2ppm .

pngle-info:
	make -C $(EXAMPLES)/pngle-info
	cp $(EXAMPLES)/pngle-info/pngle-info .

test: pngle-png2ppm pngle-info
	sh test.sh

clean:
	rm -f pngle-png2ppm pngle-info
