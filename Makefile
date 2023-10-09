.PHONY: all examples

all: tests
	make -C tests

examples:
	make -C examples

install-examples:
	make -C examples install

clean-examples:
	make -C examples clean
