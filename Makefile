.PHONY: all test examples install-examples clean-examples

all:

test: tests
	CC=cc CFLAGS=-std=c99 make -C tests clean test
	CC=cc CFLAGS=-std=c11 make -C tests clean test
	CC=c++ CFLAGS=-std=c++03 make -C tests clean test
	CC=c++ CFLAGS=-std=c++11 make -C tests clean test

examples:
	make -C examples

install-examples:
	make -C examples install

clean-examples:
	make -C examples clean
