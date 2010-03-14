#ifndef COLOR_CONV_NAIVE_H
#define COLOR_CONV_NAIVE_H

unsigned int make_rgb555_naive(unsigned int r, unsigned int g, unsigned int b);
unsigned int make_rgb565_naive(unsigned int r, unsigned int g, unsigned int b);
unsigned int make_rgb888_naive(unsigned int r, unsigned int g, unsigned int b);
unsigned int rgb555to565_naive(unsigned int rgb);
unsigned int rgb555to888_naive(unsigned int rgb);
unsigned int rgb565to555_naive(unsigned int rgb);
unsigned int rgb565to888_naive(unsigned int rgb);
unsigned int rgb888to555_naive(unsigned int rgb);
unsigned int rgb888to565_naive(unsigned int rgb);

#endif//COLOR_CONV_NAIVE_H
