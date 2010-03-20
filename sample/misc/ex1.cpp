#include "Bits.h"

#include <iostream>

using namespace emattsan::bits;

unsigned int make_rgb555(unsigned int r, unsigned int g, unsigned int b)
{
    const Bits<5> r5(r);
    const Bits<5> g5(g);
    const Bits<5> b5(b);

    return (r5, g5, b5);
}

int main(int, char* [])
{
    Bits<3, signed>   a; // ビットサイズ3の符号付き整数（-4〜3）
    Bits<5, signed>   b; // ビットサイズ5の符号付き整数（-16〜15）
    Bits<2>           c; // ビットサイズ2の符号なし整数（0〜3）
    Bits<4>           d; // ビットサイズ4の符号なし整数（0〜15）

    a = 1;  std::cout << a << std::endl;
    a = 4;  std::cout << a << std::endl;
    a = 8;  std::cout << a << std::endl;
    a = 15; std::cout << a << std::endl;
    c = 1;  std::cout << c << std::endl;
    c = 3;  std::cout << c << std::endl;
    c = 4;  std::cout << c << std::endl;
    c = 7;  std::cout << c << std::endl;

    std::cout << sizeof(Bits<1, signed char>)  << ":" << sizeof(char)  << std::endl;
    std::cout << sizeof(Bits<1, signed short>) << ":" << sizeof(short) << std::endl;
    std::cout << sizeof(Bits<1, signed int>)   << ":" << sizeof(int)   << std::endl;
    std::cout << sizeof(Bits<1, signed long>)  << ":" << sizeof(long)  << std::endl;

    Bits<4, signed> x(4); // 4 ( 0100b ) で初期化
    Bits<4, signed> y(2); // 2 ( 0010b ) で初期化

    int n = (x, y); // => 42h ( 01000010b )
    int m = (y, x); // => 24h ( 00100100b )

    std::cout
        << std::hex
        << x << std::endl
        << y << std::endl
        << n << std::endl
        << m << std::endl;

    std::cout << make_rgb555(5, 5, 5) << std::endl;

    {
        Bits<5> r, g, b;
        (r, g, b) = 0x14a5;
        std::cout << r << ":" << g << ":" << b << std::endl;
    }

    {
        Bits<2> a(1);
        Bits<2> b(1);
        int n = (a, reserve<2>, b); // n => 11h ( 010001b ) 
        std::cout << n << std::endl;
        (a, reserve<2>, b) = 0x77; // a => 3, b => 3
        std::cout << a << std::endl;
        std::cout << b << std::endl;
    }

    return 0;
}