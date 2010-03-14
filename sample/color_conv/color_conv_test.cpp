// g++ -ansi -Wall -O3 -o color_conv_test color_conv_test.cpp color_conv_naive.cpp color_conv.cpp

#include <cassert>
#include <iostream>
#include <boost/progress.hpp>

#include "color_conv.h"
#include "color_conv_naive.h"

volatile unsigned int rgb555[0x20][0x20][0x20];
volatile unsigned int rgb565[0x20][0x40][0x20];
volatile unsigned int rgb888[0x100][0x100][0x100];

void compare_make_rgb555()
{
    std::cout << "compare_make_rgb555:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x20; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                assert(make_rgb555_naive(r, g, b) == make_rgb555(r, g, b));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_make_rgb565()
{
    std::cout << "compare_make_rgb565:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x40; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                assert(make_rgb565_naive(r, g, b) == make_rgb565(r, g, b));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_make_rgb888()
{
    std::cout << "compare_make_rgb888:";
    for(unsigned int r = 0; r < 0x100; ++r)
    {
        for(unsigned int g = 0; g < 0x100; ++g)
        {
            for(unsigned int b = 0; b < 0x100; ++b)
            {
                assert(make_rgb888_naive(r, g, b) == make_rgb888(r, g, b));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb555to565()
{
    std::cout << "compare_rgb555to565:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x20; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                const unsigned int rgb = (r << 10) | (g << 5) | b;
                assert(rgb555to565_naive(rgb) == rgb555to565(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb555to888()
{
    std::cout << "compare_rgb555to888:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x20; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                const unsigned int rgb = (r << 10) | (g << 5) | b;
                assert(rgb555to888_naive(rgb) == rgb555to888(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb565to555()
{
    std::cout << "compare_rgb565to555:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x40; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                const unsigned int rgb = (r << 11) | (g << 5) | b;
                assert(rgb565to555_naive(rgb) == rgb565to555(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb565to888()
{
    std::cout << "compare_rgb565to888:";
    for(unsigned int r = 0; r < 0x20; ++r)
    {
        for(unsigned int g = 0; g < 0x40; ++g)
        {
            for(unsigned int b = 0; b < 0x20; ++b)
            {
                const unsigned int rgb = (r << 11) | (g << 5) | b;
                assert(rgb565to888_naive(rgb) == rgb565to888(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb888to555()
{
    std::cout << "compare_rgb888to555:";
    for(unsigned int r = 0; r < 0x100; ++r)
    {
        for(unsigned int g = 0; g < 0x100; ++g)
        {
            for(unsigned int b = 0; b < 0x100; ++b)
            {
                const unsigned int rgb = (r << 16) | (g << 8) | b;
                assert(rgb888to555_naive(rgb) == rgb888to555(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void compare_rgb888to565()
{
    std::cout << "compare_rgb888to565:";
    for(unsigned int r = 0; r < 0x100; ++r)
    {
        for(unsigned int g = 0; g < 0x100; ++g)
        {
            for(unsigned int b = 0; b < 0x100; ++b)
            {
                const unsigned int rgb = (r << 16) | (g << 8) | b;
                assert(rgb888to565_naive(rgb) == rgb888to565(rgb));
            }
        }
    }
    std::cout << "ok" << std::endl;
}

void test_make_rgb555()
{
    std::cout << "test_make_rgb555:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb555[r][g][b] = make_rgb555(r, g, b);
                }
            }
        }
    }
}

void test_make_rgb565()
{
    std::cout << "test_make_rgb565:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb565[r][g][b] = make_rgb555(r, g, b);
                }
            }
        }
    }
}

void test_make_rgb888()
{
    std::cout << "test_make_rgb888:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb888[r][g][b] = make_rgb555(r, g, b);
                }
            }
        }
    }
}

void test_rgb555to565()
{
    std::cout << "test_rgb555to565:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb565[r][g * 2][b] = rgb555to565(rgb555[r][g][b]);
                }
            }
        }
    }
}

void test_rgb555to888()
{
    std::cout << "test_rgb555to888:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb888[r * 8][g * 8][b * 8] = rgb555to888(rgb555[r][g][b]);
                }
            }
        }
    }
}

void test_rgb565to555()
{
    std::cout << "test_rgb565to555:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb555[r][g / 2][b] = rgb565to555(rgb565[r][g][b]);
                }
            }
        }
    }
}

void test_rgb565to888()
{
    std::cout << "test_rgb565to888:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb888[r * 8][g * 4][b * 8] = rgb565to888(rgb565[r][g][b]);
                }
            }
        }
    }
}

void test_rgb888to555()
{
    std::cout << "test_rgb888to555:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb555[r / 8][g / 8][b / 8] = rgb888to555(rgb888[r][g][b]);
                }
            }
        }
    }
}

void test_rgb888to565()
{
    std::cout << "test_rgb888to565:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb565[r / 8][g / 4][b / 8] = rgb888to565(rgb888[r][g][b]);
                }
            }
        }
    }
}

void test_make_rgb555_naive()
{
    std::cout << "test_make_rgb555_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb555[r][g][b] = make_rgb555_naive(r, g, b);
                }
            }
        }
    }
}

void test_make_rgb565_naive()
{
    std::cout << "test_make_rgb565_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb565[r][g][b] = make_rgb555_naive(r, g, b);
                }
            }
        }
    }
}

void test_make_rgb888_naive()
{
    std::cout << "test_make_rgb888_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb888[r][g][b] = make_rgb555_naive(r, g, b);
                }
            }
        }
    }
}

void test_rgb555to565_naive()
{
    std::cout << "test_rgb555to565_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb565[r][g * 2][b] = rgb555to565_naive(rgb555[r][g][b]);
                }
            }
        }
    }
}

void test_rgb555to888_naive()
{
    std::cout << "test_rgb555to888_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x20; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb888[r * 8][g * 8][b * 8] = rgb555to888_naive(rgb555[r][g][b]);
                }
            }
        }
    }
}

void test_rgb565to555_naive()
{
    std::cout << "test_rgb565to555_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb555[r][g / 2][b] = rgb565to555_naive(rgb565[r][g][b]);
                }
            }
        }
    }
}

void test_rgb565to888_naive()
{
    std::cout << "test_rgb565to888_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 1000; ++i)
    {
        for(unsigned int r = 0; r < 0x20; ++r)
        {
            for(unsigned int g = 0; g < 0x40; ++g)
            {
                for(unsigned int b = 0; b < 0x20; ++b)
                {
                    rgb888[r * 8][g * 4][b * 8] = rgb565to888_naive(rgb565[r][g][b]);
                }
            }
        }
    }
}

void test_rgb888to555_naive()
{
    std::cout << "test_rgb888to555_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb555[r / 8][g / 8][b / 8] = rgb888to555_naive(rgb888[r][g][b]);
                }
            }
        }
    }
}

void test_rgb888to565_naive()
{
    std::cout << "test_rgb888to565_naive:";
    boost::progress_timer t;

    for(int i = 0; i < 10; ++i)
    {
        for(unsigned int r = 0; r < 0x100; ++r)
        {
            for(unsigned int g = 0; g < 0x100; ++g)
            {
                for(unsigned int b = 0; b < 0x100; ++b)
                {
                    rgb565[r / 8][g / 4][b / 8] = rgb888to565_naive(rgb888[r][g][b]);
                }
            }
        }
    }
}

void test()
{
    compare_make_rgb555();
    compare_make_rgb565();
    compare_make_rgb888();
    compare_rgb555to565();
    compare_rgb555to888();
    compare_rgb565to555();
    compare_rgb565to888();
    compare_rgb888to555();
    compare_rgb888to565();

    test_make_rgb555();
    test_make_rgb565();
    test_make_rgb888();
    test_rgb555to565();
    test_rgb555to888();
    test_rgb565to555();
    test_rgb565to888();
    test_rgb888to555();
    test_rgb888to565();

    test_make_rgb555_naive();
    test_make_rgb565_naive();
    test_make_rgb888_naive();
    test_rgb555to565_naive();
    test_rgb555to888_naive();
    test_rgb565to555_naive();
    test_rgb565to888_naive();
    test_rgb888to555_naive();
    test_rgb888to565_naive();
}

int main(int, char* [])
{
    test();

    return 0;
}