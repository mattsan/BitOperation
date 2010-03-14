#include "color_conv_naive.h"

unsigned int make_rgb555_naive(unsigned int r, unsigned int g, unsigned int b)
{
    return ((r & 0x1f)) << 10 | ((g & 0x1f) << 5) | (b & 0x1f);
}

unsigned int make_rgb565_naive(unsigned int r, unsigned int g, unsigned int b)
{
    return ((r & 0x1f)) << 11 | ((g & 0x3f) << 5) | (b & 0x1f);
}

unsigned int make_rgb888_naive(unsigned int r, unsigned int g, unsigned int b)
{
    return ((r & 0xff)) << 16 | ((g & 0xff) << 8) | (b & 0xff);
}

unsigned int rgb555to565_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 10) & 0x1f;
    const unsigned int g = (rgb >>  5) & 0x1f;
    const unsigned int b =  rgb        & 0x1f;
    return (r << 11) | (g << 6) | b;
}

unsigned int rgb555to888_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 10) & 0x1f;
    const unsigned int g = (rgb >>  5) & 0x1f;
    const unsigned int b =  rgb        & 0x1f;
    return (r << 19) | (g << 11) | (b << 3);
}

unsigned int rgb565to555_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 11) & 0x1f;
    const unsigned int g = (rgb >>  5) & 0x3f;
    const unsigned int b =  rgb        & 0x1f;
    return (r << 10) | ((g >> 1) << 5) | b;
}

unsigned int rgb565to888_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 11) & 0x1f;
    const unsigned int g = (rgb >>  5) & 0x3f;
    const unsigned int b =  rgb        & 0x1f;
    return (r << 19) | (g << 10) | (b << 3);
}

unsigned int rgb888to555_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 16) & 0xff;
    const unsigned int g = (rgb >>  8) & 0xff;
    const unsigned int b =  rgb        & 0xff;
    return ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);
}

unsigned int rgb888to565_naive(unsigned int rgb)
{
    const unsigned int r = (rgb >> 16) & 0xff;
    const unsigned int g = (rgb >>  8) & 0xff;
    const unsigned int b =  rgb        & 0xff;
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}