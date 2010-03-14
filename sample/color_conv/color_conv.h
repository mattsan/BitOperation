#ifndef COLOR_CONV_H
#define COLOR_CONV_H

unsigned int make_rgb555(unsigned int r, unsigned int g, unsigned int b);
unsigned int make_rgb565(unsigned int r, unsigned int g, unsigned int b);
unsigned int make_rgb888(unsigned int r, unsigned int g, unsigned int b);
unsigned int rgb555to565(unsigned int rgb);
unsigned int rgb555to888(unsigned int rgb);
unsigned int rgb565to555(unsigned int rgb);
unsigned int rgb565to888(unsigned int rgb);
unsigned int rgb888to555(unsigned int rgb);
unsigned int rgb888to565(unsigned int rgb);

#endif//COLOR_CONV_H
