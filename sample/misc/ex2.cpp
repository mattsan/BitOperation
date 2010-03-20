#include <iostream>

#include "Bits.h"

using namespace emattsan::bits;

int main(int, char* [])
{
    Bits<4, signed> a, b, c;

    int n = 0x123;

    std::cout << std::hex;

    std::cout << "before:" << n << std::endl;

    (a, b, c) = n;

    std::cout << "a:b:c = " << a << ":" << b << ":" << c << std::endl;
    std::cout << "a:b << 2:c = " << a << ":" << (b << 2) << ":" << c << std::endl;

    n = (a, b << 2, c);

    std::cout << "after:" << n << std::endl;

    return 0;
}