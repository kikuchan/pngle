#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>

#include "pngle.h"

uint8_t *img;
int width, height;
int scale_factor = 1;

#define UNUSED(x) (void)(x)

int transparent_background = 0;
int alpha_blending = 0;

void put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	if (x < 0 || y < 0 || x >= width || y >= height) return ;

	uint8_t *p = img + (y * width + x) * 3;

	if (alpha_blending) {
		p[0] = r * a / 255 + p[0] * (255 - a) / 255;
		p[1] = g * a / 255 + p[1] * (255 - a) / 255;
		p[2] = b * a / 255 + p[2] * (255 - a) / 255;
	} else {
		p[0] = r;
		p[1] = g;
		p[2] = b;
	}
}

void rectangle(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int ix, iy;

	for (iy = 0; iy < h; iy++) {
		for (ix = 0; ix < w; ix++) {
			put_pixel(x + ix, y + iy, r, g, b, a);
		}
	}
}

void draw_pixel(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint8_t rgba[4])
{
	UNUSED(pngle);
	UNUSED(w);
	UNUSED(h);
	uint8_t r = rgba[0];
	uint8_t g = rgba[1];
	uint8_t b = rgba[2];
	uint8_t a = rgba[3];

	rectangle(x * scale_factor, y * scale_factor, scale_factor, scale_factor, r, g, b, a);
}

void init_screen(pngle_t *pngle, uint32_t w, uint32_t h)
{
	UNUSED(pngle);
	width = w * scale_factor;
	height = h * scale_factor;

	img = (uint8_t *)calloc(width * height, 3);

	if (transparent_background) {
		for (int iy = 0; iy < (height + 7) / 8; iy++) {
			for (int ix = 0; ix < (width + 7) / 8; ix++) {
				int c = (ix + iy) % 2 ? 128 : 64;;
				rectangle(ix * 8, iy * 8, 8, 8, c, c, c, 255);
			}
		}
	} else {
		const uint8_t *bg = pngle_get_background_color(pngle);
		if (bg) rectangle(0, 0, width, height, bg[0], bg[1], bg[2], 255);
	}
}

void flush_screen(pngle_t *pngle) {
	UNUSED(pngle);
	printf("P6\n");
	printf("%u %u\n", width, height);
	printf("255\n");

	fwrite(img, width * height, 3, stdout);

	// for the next image if any
	pngle_reset(pngle);
}

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	extern int optind;
	extern char *optarg;
	int ch;
	double display_gamma = 0;

	while ((ch = getopt(argc, argv, "g:s:ath")) != -1) {
		switch (ch) {
		case 'g':
			display_gamma = atof(optarg);
			break;

		case 's':
			scale_factor = atoi(optarg);
			if (scale_factor < 1) {
				fprintf(stderr, "The scale factor must be equal or greater than 1\n");
				return 1;
			}
			break;

		case 'a':
			alpha_blending = 1;
			break;

		case 't':
			transparent_background = 1;
			alpha_blending = 1; // implicit
			break;

		case 'h':
		case '?':
		default:
			fprintf(stderr, "Usage: %s [options] [input.png]\n", argv[0]);
			fprintf(stderr, "  -g [gamma] : Gamma factor\n");
			fprintf(stderr, "  -s [scale] : Scale factor (must be an integer)\n");
			fprintf(stderr, "  -a         : Enable alpha blending\n");
			fprintf(stderr, "  -t         : Transparent background (implies -a)\n");
			fprintf(stderr, "  -h         : This help\n");
			return 1;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 0) {
		fp = fopen(argv[0], "rb");
		if (fp == NULL) {
			fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
			return -1;
		}
	}

	char buf[1024];
	size_t remain = 0;
	int len;

	pngle_t *pngle = pngle_new();

	pngle_set_init_callback(pngle, init_screen);
	pngle_set_draw_callback(pngle, draw_pixel);
	pngle_set_done_callback(pngle, flush_screen);

	pngle_set_display_gamma(pngle, display_gamma);

	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
			fprintf(stderr, "Buffer exceeded\n");
			return -1;
		}

		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
			fprintf(stderr, "ERROR; %s\n", pngle_error(pngle));
			return -1;
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	if (fp != stdin) fclose(fp);

	return 0;
}
