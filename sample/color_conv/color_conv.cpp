#include "color_conv.h"
#include "Bits.h"

using namespace emattsan::bits;

unsigned int make_rgb555(unsigned int r, unsigned int g, unsigned int b)
{
    const Bits<5> r5(r);
    const Bits<5> g5(g);
    const Bits<5> b5(b);
    return (r5, g5, b5);
}

unsigned int make_rgb565(unsigned int r, unsigned int g, unsigned int b)
{
    const Bits<5> r5(r);
    const Bits<6> g6(g);
    const Bits<5> b5(b);
    return (r5, g6, b5);
}

unsigned int make_rgb888(unsigned int r, unsigned int g, unsigned int b)
{
    const Bits<8> r8(r);
    const Bits<8> g8(g);
    const Bits<8> b8(b);
    return (r8, g8, b8);
}

unsigned int rgb555to565(unsigned int rgb)
{
    Bits<5> r;
    Bits<5> g;
    Bits<5> b;
    (r, g, b) = rgb;
    return (r, g, reserve<1>, b);
}

unsigned int rgb555to888(unsigned int rgb)
{
    Bits<5> r;
    Bits<5> g;
    Bits<5> b;
    (r, g, b) = rgb;
    return (r, reserve<3>, g, reserve<3>, b, reserve<3>);
}

unsigned int rgb565to555(unsigned int rgb)
{
    Bits<5> r;
    Bits<5> g;
    Bits<5> b;
    (r, g, reserve<1>, b) = rgb;
    return (r, g, b);
}

unsigned int rgb565to888(unsigned int rgb)
{
    Bits<5> r;
    Bits<6> g;
    Bits<5> b;
    (r, g, b) = rgb;
    return (r, reserve<3>, g, reserve<2>, b, reserve<3>);
}

unsigned int rgb888to555(unsigned int rgb)
{
    Bits<5> r;
    Bits<5> g;
    Bits<5> b;
    (r, reserve<3>, g, reserve<3>, b, reserve<3>) = rgb;
    return (r, g, b);
}

unsigned int rgb888to565(unsigned int rgb)
{
    Bits<5> r;
    Bits<6> g;
    Bits<5> b;
    (r, reserve<3>, g, reserve<2>, b, reserve<3>) = rgb;
    return (r, g, b);
}