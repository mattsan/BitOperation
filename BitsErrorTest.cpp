#include "Bits.h"

using namespace emattsan::bits;

void test()
{
    Bits<1> u1; // OK
//    Bits<0> u2;  // compile error: can not use zero fo 1st template parameter
//    Bits<-1> u3; // compile error: can not use negative number fo 1st template parameter

    Bits<std::numeric_limits<unsigned int>::digits> u4; // OK
//    Bits<std::numeric_limits<unsigned int>::digits + 1> u5; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned long>::digits, unsigned long> u6; // OK
//    Bits<std::numeric_limits<unsigned long>::digits + 1, unsigned long> u7; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned short>::digits, unsigned short> u8; // OK
//    Bits<std::numeric_limits<unsigned short>::digits + 1, unsigned short> u9; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned char>::digits, unsigned char> u10; // OK
//    Bits<std::numeric_limits<unsigned char>::digits + 1, unsigned char> u11; // compile error: can not use greater than container's digits size

    Bits<1, signed> s1; // OK
//    Bits<0, signed> s2;  // compile error: can not use zero fo 1st template parameter
//    Bits<-1, signed> s3; // compile error: can not use negative number fo 1st template parameter

    Bits<std::numeric_limits<unsigned >::digits, signed> s4; // OK
//    Bits<std::numeric_limits<unsigned int>::digits + 1, signed> s5; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned long>::digits, long> s6; // OK
//    Bits<std::numeric_limits<unsigned long>::digits + 1, long> s7; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned short>::digits, short> s8; // OK
//    Bits<std::numeric_limits<unsigned short>::digits + 1, short> s9; // compile error: can not use greater than container's digits size

    Bits<std::numeric_limits<unsigned char>::digits, signed char> s10; // OK
//    Bits<std::numeric_limits<unsigned char>::digits + 1, unsigned char> s11; // compile error: can not use greater than container's digits size
}

int main(int, char* [])
{
    test();

    return 0;
}