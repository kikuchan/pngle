.PHONY: all pngle-info pngle-png2ppm install clean

all: pngle-info pngle-png2ppm

pngle-info:
	make -C pngle-info

pngle-png2ppm:
	make -C pngle-png2ppm

install: pngle-info pngle-png2ppm
	make -C pngle-info install
	make -C pngle-png2ppm install

clean:
	make -C pngle-info clean
	make -C pngle-png2ppm clean
