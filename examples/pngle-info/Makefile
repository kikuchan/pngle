CFLAGS+=-Os
CFLAGS+=-I../../src
CFLAGS+=-Wall -Wextra -Wno-misleading-indentation -Wno-unknown-warning-option
# CFLAGS += -DPNGLE_NO_GAMMA_CORRECTION

PNGLE_C=../../src/pngle.c ../../src/miniz.c
PNGLE_H=../../src/pngle.h ../../src/miniz.h

SRC=pngle-info.c
PROG=pngle-info
LIBS=-lm

PREFIX?=/usr/local

all: $(PROG)

$(PROG): $(SRC) $(PNGLE_C) $(PNGLE_H) Makefile
	$(CC) $(CFLAGS) -o $(PROG) $(SRC) $(PNGLE_C) $(LIBS)

install: $(PROG)
	install $(PROG) $(PREFIX)/bin

clean:
	rm -f $(PROG)
