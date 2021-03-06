// compile: g++ -Wall -o BitsTest BitsTest.cpp -lgtest
// need Google Test (see: http://code.google.com/p/googletest/ )

#include <gtest/gtest.h>

#include "Bits.h"

using namespace emattsan::bits;

// 各々のビット列のサイズが設定された値になっていること
TEST(SizeTest, Test0)
{
    Bits<1, signed> sbits1;
    ASSERT_EQ(1, sbits1.size());

    Bits<4, signed> sbits4;
    ASSERT_EQ(4, sbits4.size());

    Bits<1> ubits1;
    ASSERT_EQ(1, ubits1.size());

    Bits<4> ubits4;
    ASSERT_EQ(4, ubits4.size());
}

// 符号付きビット列で初期化ができること
TEST(SignedBitsTest, InitTest)
{
    Bits<4, signed> bits1;
    ASSERT_EQ(0, bits1.get());

    Bits<4, signed> bits2(7);
    ASSERT_EQ(7, bits2.get());

    Bits<4, signed> bits3(-1);
    ASSERT_EQ(-1, bits3.get());

    Bits<4, signed> bits4(-8);
    ASSERT_EQ(-8, bits4.get());

    // 符号付き4bitは格納できる値の範囲は-8〜7なので、8で初期化するとサイクリックして-8になる
    Bits<4, signed> bits5(8);
    ASSERT_EQ(-8, bits5.get()); // cycled
}

// 符号付きビット列で値を設定できること
TEST(SignedBitsTest, SetTest)
{
    Bits<3, signed> bits;
    ASSERT_EQ(0, bits.get());

    bits.set(1);
    ASSERT_EQ(1, bits.get());

    bits.set(2);
    ASSERT_EQ(2, bits.get());

    bits.set(3);
    ASSERT_EQ(3, bits.get());

    bits.set(4);
    ASSERT_EQ(-4, bits.get()); // cycled

    bits.set(5);
    ASSERT_EQ(-3, bits.get()); // cycled

    bits.set(-3);
    ASSERT_EQ(-3, bits.get());

    bits.set(-2);
    ASSERT_EQ(-2, bits.get());

    bits.set(-1);
    ASSERT_EQ(-1, bits.get());

    bits.set(0);
    ASSERT_EQ(0, bits.get());
}

// 符号付きビット列で値を代入できること
TEST(SignedBitsTest, AssignTest)
{
    Bits<3, signed> bits;
    ASSERT_EQ(0, static_cast<int>(bits));

    bits = 1;
    ASSERT_EQ(1, static_cast<int>(bits));

    bits = 2;
    ASSERT_EQ(2, static_cast<int>(bits));

    bits = 3;
    ASSERT_EQ(3, static_cast<int>(bits));

    bits = 4;
    ASSERT_EQ(-4, static_cast<int>(bits)); // cycled

    bits = 5;
    ASSERT_EQ(-3, static_cast<int>(bits)); // cycled

    bits = -3;
    ASSERT_EQ(-3, static_cast<int>(bits));

    bits = -2;
    ASSERT_EQ(-2, static_cast<int>(bits));

    bits = -1;
    ASSERT_EQ(-1, static_cast<int>(bits));

    bits = 0;
    ASSERT_EQ(0, static_cast<int>(bits));
}

// 符号なしビット列で初期化ができること
TEST(UnsignedBitsTest, InitTest1)
{
    Bits<4> bits1;
    ASSERT_EQ(0u, bits1.get());

    Bits<4> bits2(1u);
    ASSERT_EQ(1u, bits2.get());

    Bits<4> bits3(7u);
    ASSERT_EQ(7u, bits3.get());

    Bits<4> bits4(8u);
    ASSERT_EQ(8u, bits4.get());

    Bits<4> bits5(15u);
    ASSERT_EQ(15u, bits5.get());
}

// 符号なしビット列で初期化ができること（サイクリックする場合）
TEST(UnsignedBitsTest, InitTest2)
{
    Bits<4> bits1(16u);
    ASSERT_EQ(0u, bits1.get()); // cycled

    Bits<4> bits2(17u);
    ASSERT_EQ(1u, bits2.get()); // cycled

    Bits<4> bits3(23u);
    ASSERT_EQ(7u, bits3.get()); // cycled

    Bits<4> bits4(24u);
    ASSERT_EQ(8u, bits4.get()); // cycled

    Bits<4> bits5(31u);
    ASSERT_EQ(15u, bits5.get()); // cycled
}

// 符号なしビット列で値を設定できること
TEST(UnsignedBitsTest, SetTest)
{
    Bits<3> bits;
    ASSERT_EQ(0u, bits.get());

    bits.set(1u);
    ASSERT_EQ(1u, bits.get());

    bits.set(4u);
    ASSERT_EQ(4u, bits.get());

    bits.set(7u);
    ASSERT_EQ(7u, bits.get());

    bits.set(8u);
    ASSERT_EQ(0u, bits.get()); // cycled

    bits.set(15u);
    ASSERT_EQ(7u, bits.get()); // cycled
}

// 符号なしビット列で値を代入できること
TEST(UnsignedBitsTest, AssignTest)
{
    Bits<3> bits;
    ASSERT_EQ(0u, static_cast<unsigned int>(bits));

    bits = 1u;
    ASSERT_EQ(1u, static_cast<unsigned int>(bits));

    bits = 4u;
    ASSERT_EQ(4u, static_cast<unsigned int>(bits));

    bits = 7u;
    ASSERT_EQ(7u, static_cast<unsigned int>(bits));

    bits = 8u;
    ASSERT_EQ(0u, static_cast<unsigned int>(bits)); // cycled

    bits = 15u;
    ASSERT_EQ(7u, static_cast<unsigned int>(bits)); // cycled
}

// 連結したビット列の長さが、元のビット列の長さの和になること
TEST(PackTest, SizeTest)
{
    Bits<4, signed> sbits4;
    Bits<3, signed> sbits3;

    ASSERT_EQ(7, (sbits4, sbits3).size());
}

// 連結したビット列から値を得られること
TEST(PackTest, GetTest)
{
    Bits<4, signed> sbits4;
    Bits<3, signed> sbits3;

    sbits4 = 1;
    sbits3 = 2;

    ASSERT_EQ(10, static_cast<int>(sbits4, sbits3));
    ASSERT_EQ(33, static_cast<int>(sbits3, sbits4));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（２つのビット列の連結）
TEST(PackTest, SetTest1)
{
    Bits<4, signed> sbits4;
    Bits<3, signed> sbits3;

    ASSERT_EQ(0, static_cast<int>(sbits4));
    ASSERT_EQ(0, static_cast<int>(sbits3));

    (sbits4, sbits3) = 1u;

    ASSERT_EQ(1, static_cast<int>(sbits3));
    ASSERT_EQ(0, static_cast<int>(sbits4));

    (sbits4, sbits3) = 8u;

    ASSERT_EQ(0, static_cast<int>(sbits3));
    ASSERT_EQ(1, static_cast<int>(sbits4));

    (sbits4, sbits3) = 15u;

    ASSERT_EQ(-1, static_cast<int>(sbits3));
    ASSERT_EQ(1, static_cast<int>(sbits4));

    (sbits4, sbits3) = 7u << 3 | 3u;

    ASSERT_EQ(3, static_cast<int>(sbits3));
    ASSERT_EQ(7, static_cast<int>(sbits4));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（３つのビット列の連結）
TEST(PackTest, SetTest2)
{
    Bits<2> ubits1;
    Bits<3> ubits2;
    Bits<3> ubits3;

    ASSERT_EQ(0u, static_cast<unsigned int>(ubits1));
    ASSERT_EQ(0u, static_cast<unsigned int>(ubits2));
    ASSERT_EQ(0u, static_cast<unsigned int>(ubits3));

    (ubits1, ubits2, ubits3) = 255u;

    ASSERT_EQ(3u, static_cast<unsigned int>(ubits1));
    ASSERT_EQ(7u, static_cast<unsigned int>(ubits2));
    ASSERT_EQ(7u, static_cast<unsigned int>(ubits3));

    (ubits1, ubits2, ubits3) = 1u << 6 | 3u << 3 | 3u;

    ASSERT_EQ(1u, static_cast<unsigned int>(ubits1));
    ASSERT_EQ(3u, static_cast<unsigned int>(ubits2));
    ASSERT_EQ(3u, static_cast<unsigned int>(ubits3));
}

// 連結したビット列から値を得られること（定数ビット列の連結）
TEST(PackTest, ConstGetTest)
{
    const Bits<4, signed> highnibble(5);
    const Bits<4, signed> lownibble(10);

    int n = (highnibble, lownibble);

    ASSERT_EQ(0x5a, n);
}

// 連結したビット列から値を得られること（定数と変数が混在したビット列の連結）
TEST(PackTest, MixedGetTest1)
{
    Bits<2, signed>       bits1(1);
    Bits<2, signed>       bits2(1);
    const Bits<2, signed> bits3(1);

    int n = (bits1, bits2, bits3);

    ASSERT_EQ(0x15, n);

    bits1 = 2;
    bits2 = 2;
    n     = (bits1, bits2, bits3);

    ASSERT_EQ(0x29, n);

    n     = (bits1, (bits2, bits3));

    ASSERT_EQ(0x29, n);
}

// 連結したビット列から値を得られること（符号付きと符号なしが混在したビット列の連結）
TEST(PackTest, MixedGetTest2)
{
    Bits<4, signed>   sbits1(1);
    Bits<4, signed>   sbits2(1);
    Bits<4> ubits1(1);
    Bits<4> ubits2(1);

    ASSERT_EQ(0x1111, static_cast<int>(sbits1, sbits2, ubits1, ubits2));
}

// 連結したビット列から値を得られること（異なるブロックサイズ（内部でビット列を格納する型）が混在したビット列の連結）
TEST(PackTest, MixedGetTest3)
{
    Bits<2, signed char> sbits1(1);
    Bits<6, signed int>  sbits2(1);

    ASSERT_EQ(0x41, static_cast<int>(sbits1, sbits2));

    Bits<12, signed short> sbits3(1);

    ASSERT_EQ(0x41001, static_cast<int>(sbits1, sbits2, sbits3));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（符号付きと符号なしが混在したビット列の連結）
TEST(PackTest, MixedSetTest)
{
    Bits<4, signed>   sbits1;
    Bits<4, signed>   sbits2;
    Bits<4> ubits1;
    Bits<4> ubits2;

    (sbits1, sbits2, ubits1, ubits2) = 0x89ab;

    ASSERT_EQ(-8, static_cast<int>(sbits1));
    ASSERT_EQ(-7, static_cast<int>(sbits2));
    ASSERT_EQ(10u, static_cast<unsigned int>(ubits1));
    ASSERT_EQ(11u, static_cast<unsigned int>(ubits2));
}

// 連結したビット列から値を得られること（利用しない領域を含む）
TEST(ReservedBitsTest, GetTest1)
{
    Bits<3, signed> bits1(2);
    Bits<3, signed> bits2(3);

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));

    // +----+----+----+----+----+----+----+----+
    // |     bits1    |reserved |     bits2    |
    // +----+----+----+----+----+----+----+----+

    ASSERT_EQ(0x43, static_cast<int>(bits1, reserve<2>, bits2));
}

// 連結したビット列から値を得られること（利用しない領域を含む）（3要素）
TEST(ReservedBitsTest, GetTest2)
{
    Bits<3, signed> bits1(2);
    Bits<3, signed> bits2(3);
    Bits<3, signed> bits3(4);

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));
    ASSERT_EQ(-4, static_cast<int>(bits3));

    ASSERT_EQ(0x434, static_cast<int>(bits1, reserve<2>, bits2, reserve<1>, bits3));
}

// 連結したビット列から値を得られること（利用しない領域を含む）（3要素）（定数）
TEST(ReservedBitsTest, GetTest3)
{
    const Bits<3, signed> bits1(2);
    const Bits<3, signed> bits2(3);
    const Bits<3, signed> bits3(4);

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));
    ASSERT_EQ(-4, static_cast<int>(bits3));

    ASSERT_EQ(0x434, static_cast<int>(bits1, reserve<2>, bits2, reserve<1>, bits3));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（利用しない領域を含む）
TEST(ReservedBitsTest, SetTest1)
{
    Bits<3, signed> bits1;
    Bits<3, signed> bits2;

    ASSERT_EQ(0, static_cast<int>(bits1));
    ASSERT_EQ(0, static_cast<int>(bits2));

    (bits1, reserve<2>, bits2) = 0x43;

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（利用しない領域を含む）（3要素）
TEST(ReservedBitsTest, SetTest2)
{
    Bits<3, signed> bits1;
    Bits<3, signed> bits2;
    Bits<3, signed> bits3;

    ASSERT_EQ(0, static_cast<int>(bits1));
    ASSERT_EQ(0, static_cast<int>(bits2));
    ASSERT_EQ(0, static_cast<int>(bits3));

    (bits1, reserve<2>, bits2, reserve<1>, bits3) = 0x434;

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));
    ASSERT_EQ(-4, static_cast<int>(bits3));
}

// ビット列型のサイズが値を格納する整数型のサイズと等しいこと
TEST(SizeTest, Test1)
{
    ASSERT_EQ(sizeof(char),  sizeof(Bits<1, signed char>));
    ASSERT_EQ(sizeof(short), sizeof(Bits<1, signed short>));
    ASSERT_EQ(sizeof(int),   sizeof(Bits<1, signed int>));
    ASSERT_EQ(sizeof(long),  sizeof(Bits<1, signed long>));
    ASSERT_EQ(sizeof(char),  sizeof(Bits<1, unsigned char>));
    ASSERT_EQ(sizeof(short), sizeof(Bits<1, unsigned short>));
    ASSERT_EQ(sizeof(int),   sizeof(Bits<1, unsigned int>));
    ASSERT_EQ(sizeof(long),  sizeof(Bits<1, unsigned long>));
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（１）
TEST(SizeTest, Test2)
{
    Bits<4, signed char> bits1;

    ASSERT_EQ(sizeof(char), sizeof((bits1, bits1).getSequence()));
    ASSERT_EQ(sizeof(short), sizeof((bits1, bits1, bits1).getSequence()));
    ASSERT_EQ(sizeof(short), sizeof((bits1, bits1, bits1).getSequence()));
    ASSERT_EQ(sizeof(short), sizeof((bits1, bits1, bits1, bits1).getSequence()));
    ASSERT_EQ(sizeof(int), sizeof((bits1, bits1, bits1, bits1, bits1).getSequence()));
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（２）
TEST(SizeTest, Test3)
{
    Bits<2, signed char> bits1;

    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits1, bits1, bits1, bits1).getSequence());
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（３）
TEST(SizeTest, Test4)
{
    Bits<2, signed char> bits1;

    ASSERT_EQ(sizeof(char), sizeof(bits1, reserve<6>).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, reserve<7>).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, reserve<14>).getSequence());
    ASSERT_EQ(sizeof(int), sizeof(bits1, reserve<15>).getSequence());
    ASSERT_EQ(sizeof(int), sizeof(bits1, reserve<30>).getSequence());
}

// 境界テスト：charをコンテナ型にした場合に符号付き8ビットの値を格納できること
TEST(BoundaryTest, SignedTest1)
{
    Bits<8, signed char> s8;
    ASSERT_EQ(0, static_cast<int>(s8));

    s8 = 127;
    ASSERT_EQ(127, static_cast<int>(s8));

    s8 = 128;
    ASSERT_EQ(-128, static_cast<int>(s8));

    s8 = 255;
    ASSERT_EQ(-1, static_cast<int>(s8));

    s8 = 256;
    ASSERT_EQ(0, static_cast<int>(s8));
}

// 境界テスト：shortをコンテナ型にした場合に符号付き16ビットの値を格納できること
TEST(BoundaryTest, SignedTest2)
{
    Bits<16, signed short> s16;
    ASSERT_EQ(0, static_cast<int>(s16));

    s16 = 32767;
    ASSERT_EQ(32767, static_cast<int>(s16));

    s16 = 32768;
    ASSERT_EQ(-32768, static_cast<int>(s16));

    s16 = 65535;
    ASSERT_EQ(-1, static_cast<int>(s16));

    s16 = 65536;
    ASSERT_EQ(0, static_cast<int>(s16));
}

// 境界テスト：intをコンテナ型にした場合に符号付き32ビットの値を格納できること
TEST(BoundaryTest, SignedTest3)
{
    Bits<32, signed int> s32;
    ASSERT_EQ(0, static_cast<int>(s32));

    s32 = 2147483647;
    ASSERT_EQ(2147483647, static_cast<int>(s32));

    s32 = 0x80000000;
    ASSERT_EQ(static_cast<int>(0x80000000), static_cast<int>(s32));

    s32 = -1;
    ASSERT_EQ(-1, static_cast<int>(s32));

    s32 = 0;
    ASSERT_EQ(0, static_cast<int>(s32));
}

// 境界テスト：longをコンテナ型にした場合に符号付き32ビットの値を格納できること
TEST(BoundaryTest, SignedTest4)
{
    Bits<32, signed long> s32;
    ASSERT_EQ(0, static_cast<int>(s32));

    s32 = 2147483647;
    ASSERT_EQ(2147483647, static_cast<int>(s32));

    s32 = 0x80000000;
    ASSERT_EQ(static_cast<int>(0x80000000), static_cast<int>(s32));

    s32 = -1;
    ASSERT_EQ(-1, static_cast<int>(s32));

    s32 = 0;
    ASSERT_EQ(0, static_cast<int>(s32));
}

// 境界テスト：charをコンテナ型にした場合に符号なし8ビットの値を格納できること
TEST(BoundaryTest, UnsignedTest1)
{
    Bits<8, unsigned char> u8;
    ASSERT_EQ(0u, static_cast<unsigned int>(u8));

    u8 = 127u;
    ASSERT_EQ(127u, static_cast<unsigned int>(u8));

    u8 = 128u;
    ASSERT_EQ(128u, static_cast<unsigned int>(u8));

    u8 = 255u;
    ASSERT_EQ(255u, static_cast<unsigned int>(u8));

    u8 = 256u;
    ASSERT_EQ(0u, static_cast<unsigned int>(u8));
}

// 境界テスト：shortをコンテナ型にした場合に符号なし16ビットの値を格納できること
TEST(BoundaryTest, UnsignedTest2)
{
    Bits<16, unsigned short> u16;
    ASSERT_EQ(0u, static_cast<unsigned int>(u16));

    u16 = 32767u;
    ASSERT_EQ(32767u, static_cast<unsigned int>(u16));

    u16 = 32768u;
    ASSERT_EQ(32768u, static_cast<unsigned int>(u16));

    u16 = 65535u;
    ASSERT_EQ(65535u, static_cast<unsigned int>(u16));

    u16 = 65536u;
    ASSERT_EQ(0u, static_cast<unsigned int>(u16));
}

// 境界テスト：intをコンテナ型にした場合に符号なし32ビットの値を格納できること
TEST(BoundaryTest, UnsignedTest3)
{
    Bits<32, unsigned int> u32;
    ASSERT_EQ(0u, static_cast<unsigned int>(u32));

    u32 = 2147483647u;
    ASSERT_EQ(2147483647u, static_cast<unsigned int>(u32));

    u32 = 0x80000000u;
    ASSERT_EQ(0x80000000u, static_cast<unsigned int>(u32));

    u32 = static_cast<unsigned int>(-1);
    ASSERT_EQ(0xffffffffu, static_cast<unsigned int>(u32));

    u32 = 0u;
    ASSERT_EQ(0u, static_cast<unsigned int>(u32));
}

// 境界テスト：longをコンテナ型にした場合に符号なし32ビットの値を格納できること
TEST(BoundaryTest, UnsignedTest4)
{
    Bits<32, unsigned long> u32;
    ASSERT_EQ(0u, static_cast<unsigned int>(u32));

    u32 = 2147483647u;
    ASSERT_EQ(2147483647u, static_cast<unsigned int>(u32));

    u32 = 0x80000000u;
    ASSERT_EQ(0x80000000u, static_cast<unsigned int>(u32));

    u32 = static_cast<unsigned long>(-1);
    ASSERT_EQ(0xffffffffu, static_cast<unsigned int>(u32));

    u32 = 0u;
    ASSERT_EQ(0u, static_cast<unsigned int>(u32));
}

TEST(CalcTest, AddTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 += 10;
    ASSERT_EQ(20, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 += 10;
    ASSERT_EQ(20, static_cast<int>(u6));
}

TEST(CalcTest, AddTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 + s4).size());
    ASSERT_EQ(6, (s4 + s6).size());
    ASSERT_EQ(20, static_cast<int>(s6 + s4));
    ASSERT_EQ(20, static_cast<int>(s4 + s6));

    s6 = 31;
    ASSERT_EQ(-26, static_cast<int>(s6 + s4));
    ASSERT_EQ(-26, static_cast<int>(s4 + s6));
}

TEST(CalcTest, AddTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 + 3).size());
    ASSERT_EQ(4, (3 + s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 + 3));
    ASSERT_EQ(7, static_cast<int>(3 + s4));

    ASSERT_EQ(4, (s4 + 4).size());
    ASSERT_EQ(4, (4 + s4).size());
    ASSERT_EQ(-8, static_cast<int>(s4 + 4));
    ASSERT_EQ(-8, static_cast<int>(4 + s4));

    ASSERT_EQ(4, (s4 + 3u).size());
    ASSERT_EQ(4, (3u + s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 + 3u));
    ASSERT_EQ(7, static_cast<int>(3u + s4));

    ASSERT_EQ(4, (s4 + 4u).size());
    ASSERT_EQ(4, (4u + s4).size());
    ASSERT_EQ(8, static_cast<int>(s4 + 4u));
    ASSERT_EQ(8, static_cast<int>(4u + s4));
}

TEST(CalcTest, AddTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _4(4);
    const Bits<4> _3u(3);
    const Bits<4> _4u(4);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 + _3).size());
    ASSERT_EQ(4, (_3 + s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 + _3));
    ASSERT_EQ(7, static_cast<int>(_3 + s4));

    ASSERT_EQ(4, (s4 + _4).size());
    ASSERT_EQ(4, (_4 + s4).size());
    ASSERT_EQ(-8, static_cast<int>(s4 + _4));
    ASSERT_EQ(-8, static_cast<int>(_4 + s4));

    ASSERT_EQ(4, (s4 + _3u).size());
    ASSERT_EQ(4, (_3u + s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 + _3u));
    ASSERT_EQ(7, static_cast<int>(_3u + s4));

    ASSERT_EQ(4, (s4 + _4u).size());
    ASSERT_EQ(4, (_4u + s4).size());
    ASSERT_EQ(8, static_cast<int>(s4 + _4u));
    ASSERT_EQ(8, static_cast<int>(_4u + s4));
}

TEST(CalcTest, AddTest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 + 3).size());
    ASSERT_EQ(4, (3 + u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 + 3));
    ASSERT_EQ(7, static_cast<int>(3 + u4));

    ASSERT_EQ(4, (u4 + 4).size());
    ASSERT_EQ(4, (4 + u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 + 4));
    ASSERT_EQ(8, static_cast<int>(4 + u4));

    ASSERT_EQ(4, (u4 + 3u).size());
    ASSERT_EQ(4, (3u + u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 + 3u));
    ASSERT_EQ(7, static_cast<int>(3u + u4));

    ASSERT_EQ(4, (u4 + 4u).size());
    ASSERT_EQ(4, (4u + u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 + 4u));
    ASSERT_EQ(8, static_cast<int>(4u + u4));
}

TEST(CalcTest, AddTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _4(4);
    const Bits<4> _3u(3);
    const Bits<4> _4u(4);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 + _3).size());
    ASSERT_EQ(4, (_3 + u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 + _3));
    ASSERT_EQ(7, static_cast<int>(_3 + u4));

    ASSERT_EQ(4, (u4 + _4).size());
    ASSERT_EQ(4, (_4 + u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 + _4));
    ASSERT_EQ(8, static_cast<int>(_4 + u4));

    ASSERT_EQ(4, (u4 + _3u).size());
    ASSERT_EQ(4, (_3u + u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 + _3u));
    ASSERT_EQ(7, static_cast<int>(_3u + u4));

    ASSERT_EQ(4, (u4 + _4u).size());
    ASSERT_EQ(4, (_4u + u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 + _4u));
    ASSERT_EQ(8, static_cast<int>(_4u + u4));
}

TEST(CalcTest, SubTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 -= 5;
    ASSERT_EQ(5, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 -= 5;
    ASSERT_EQ(5, static_cast<int>(u6));
}

TEST(CalcTest, SubTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 - s4).size());
    ASSERT_EQ(6, (s4 - s6).size());
    ASSERT_EQ(6, static_cast<int>(s6 - s4));
    ASSERT_EQ(-6, static_cast<int>(s4 - s6));

    s6 = 31;
    ASSERT_EQ(24, static_cast<int>(s6 - s4));
    ASSERT_EQ(-24, static_cast<int>(s4 - s6));
}

TEST(CalcTest, SubTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 - 3).size());
    ASSERT_EQ(4, (3 - s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 - 3));
    ASSERT_EQ(-1, static_cast<int>(3 - s4));

    ASSERT_EQ(4, (s4 - 5).size());
    ASSERT_EQ(4, (5 - s4).size());
    ASSERT_EQ(-1, static_cast<int>(s4 - 5));
    ASSERT_EQ(1, static_cast<int>(5 - s4));

    ASSERT_EQ(4, (s4 - 3u).size());
    ASSERT_EQ(4, (3u - s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 - 3u));
    ASSERT_EQ(15, static_cast<int>(3u - s4));

    ASSERT_EQ(4, (s4 - 5u).size());
    ASSERT_EQ(4, (5u - s4).size());
    ASSERT_EQ(15, static_cast<int>(s4 - 5u));
    ASSERT_EQ(1, static_cast<int>(5u - s4));
}

TEST(CalcTest, SubTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 - _3).size());
    ASSERT_EQ(4, (_3 - s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 - _3));
    ASSERT_EQ(-1, static_cast<int>(_3 - s4));

    ASSERT_EQ(4, (s4 - _5).size());
    ASSERT_EQ(4, (_5 - s4).size());
    ASSERT_EQ(-1, static_cast<int>(s4 - _5));
    ASSERT_EQ(1, static_cast<int>(_5 - s4));

    ASSERT_EQ(4, (s4 - _3u).size());
    ASSERT_EQ(4, (_3u - s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 - _3u));
    ASSERT_EQ(15, static_cast<int>(_3u - s4));

    ASSERT_EQ(4, (s4 - _5u).size());
    ASSERT_EQ(4, (_5u - s4).size());
    ASSERT_EQ(15, static_cast<int>(s4 - _5u));
    ASSERT_EQ(1, static_cast<int>(_5u - s4));
}

TEST(CalcTest, SubTest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 - 3).size());
    ASSERT_EQ(4, (3 - u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 - 3));
    ASSERT_EQ(15, static_cast<int>(3 - u4));

    ASSERT_EQ(4, (u4 - 5).size());
    ASSERT_EQ(4, (5 - u4).size());
    ASSERT_EQ(15, static_cast<int>(u4 - 5));
    ASSERT_EQ(1, static_cast<int>(5 - u4));

    ASSERT_EQ(4, (u4 - 3u).size());
    ASSERT_EQ(4, (3u - u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 - 3u));
    ASSERT_EQ(15, static_cast<int>(3u - u4));

    ASSERT_EQ(4, (u4 - 5u).size());
    ASSERT_EQ(4, (5u - u4).size());
    ASSERT_EQ(15, static_cast<int>(u4 - 5u));
    ASSERT_EQ(1, static_cast<int>(5u - u4));
}

TEST(CalcTest, SubTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 - _3).size());
    ASSERT_EQ(4, (_3 - u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 - _3));
    ASSERT_EQ(15, static_cast<int>(_3 - u4));

    ASSERT_EQ(4, (u4 - _5).size());
    ASSERT_EQ(4, (_5 - u4).size());
    ASSERT_EQ(15, static_cast<int>(u4 - _5));
    ASSERT_EQ(1, static_cast<int>(_5 - u4));

    ASSERT_EQ(4, (u4 - _3u).size());
    ASSERT_EQ(4, (_3u - u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 - _3u));
    ASSERT_EQ(15, static_cast<int>(_3u - u4));

    ASSERT_EQ(4, (u4 - _5u).size());
    ASSERT_EQ(4, (_5u - u4).size());
    ASSERT_EQ(15, static_cast<int>(u4 - _5u));
    ASSERT_EQ(1, static_cast<int>(_5u - u4));
}

TEST(CalcTest, MulTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 *= 5; // 50 = 110010b => -001110b => -14 
    ASSERT_EQ(-14, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 *= 5;
    ASSERT_EQ(50, static_cast<int>(u6));
}

TEST(CalcTest, MulTest2)
{
    Bits<6, signed> s6(15);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 * s4).size());
    ASSERT_EQ(6, (s4 * s6).size());
    ASSERT_EQ(-23, static_cast<int>(s6 * s4)); // 15 * 7 = 105 = 1101001b => 101001b => -010111b => -23
    ASSERT_EQ(-23, static_cast<int>(s4 * s6));

    s6 = 31;
    ASSERT_EQ(25, static_cast<int>(s6 * s4)); // 31 * 7 = 217 = 11011001 => 011001b => 25
    ASSERT_EQ(25, static_cast<int>(s4 * s6));
}

TEST(CalcTest, MulTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 * 3).size());
    ASSERT_EQ(4, (3 * s4).size());
    ASSERT_EQ(-4, static_cast<int>(s4 * 3));
    ASSERT_EQ(-4, static_cast<int>(3 * s4));

    ASSERT_EQ(4, (s4 * 5).size());
    ASSERT_EQ(4, (5 * s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 * 5));
    ASSERT_EQ(4, static_cast<int>(5 * s4));

    ASSERT_EQ(4, (s4 * 3u).size());
    ASSERT_EQ(4, (3u * s4).size());
    ASSERT_EQ(12, static_cast<int>(s4 * 3u));
    ASSERT_EQ(12, static_cast<int>(3u * s4));

    ASSERT_EQ(4, (s4 * 5u).size());
    ASSERT_EQ(4, (5u * s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 * 5u));
    ASSERT_EQ(4, static_cast<int>(5u * s4));
}

TEST(CalcTest, MulTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 * _3).size());
    ASSERT_EQ(4, (_3 * s4).size());
    ASSERT_EQ(-4, static_cast<int>(s4 * _3));
    ASSERT_EQ(-4, static_cast<int>(_3 * s4));

    ASSERT_EQ(4, (s4 * _5).size());
    ASSERT_EQ(4, (_5 * s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 * _5));
    ASSERT_EQ(4, static_cast<int>(_5 * s4));

    ASSERT_EQ(4, (s4 * _3u).size());
    ASSERT_EQ(4, (_3u * s4).size());
    ASSERT_EQ(12, static_cast<int>(s4 * _3u));
    ASSERT_EQ(12, static_cast<int>(_3u * s4));

    ASSERT_EQ(4, (s4 * _5u).size());
    ASSERT_EQ(4, (_5u * s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 * _5u));
    ASSERT_EQ(4, static_cast<int>(_5u * s4));
}

TEST(CalcTest, MulTest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 * 3).size());
    ASSERT_EQ(4, (3 * u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 * 3));
    ASSERT_EQ(12, static_cast<int>(3 * u4));

    ASSERT_EQ(4, (u4 * 5).size());
    ASSERT_EQ(4, (5 * u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 * 5));
    ASSERT_EQ(4, static_cast<int>(5 * u4));

    ASSERT_EQ(4, (u4 * 3u).size());
    ASSERT_EQ(4, (3u * u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 * 3u));
    ASSERT_EQ(12, static_cast<int>(3u * u4));

    ASSERT_EQ(4, (u4 * 5u).size());
    ASSERT_EQ(4, (5u * u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 * 5u));
    ASSERT_EQ(4, static_cast<int>(5u * u4));
}

TEST(CalcTest, MulTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 * _3).size());
    ASSERT_EQ(4, (_3 * u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 * _3));
    ASSERT_EQ(12, static_cast<int>(_3 * u4));

    ASSERT_EQ(4, (u4 * _5).size());
    ASSERT_EQ(4, (_5 * u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 * _5));
    ASSERT_EQ(4, static_cast<int>(_5 * u4));

    ASSERT_EQ(4, (u4 * _3u).size());
    ASSERT_EQ(4, (_3u * u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 * _3u));
    ASSERT_EQ(12, static_cast<int>(_3u * u4));

    ASSERT_EQ(4, (u4 * _5u).size());
    ASSERT_EQ(4, (_5u * u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 * _5u));
    ASSERT_EQ(4, static_cast<int>(_5u * u4));
}

TEST(CalcTest, DivTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 /= 3;
    ASSERT_EQ(3, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 /= -3;
    ASSERT_EQ(-3, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 /= 3;
    ASSERT_EQ(3, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 /= -3; // -3 = 111101b => 61 => 10/61=0
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(CalcTest, DivTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 / s4).size());
    ASSERT_EQ(6, (s4 / s6).size());
    ASSERT_EQ(1, static_cast<int>(s6 / s4));
    ASSERT_EQ(0, static_cast<int>(s4 / s6));

    s6 = 31;
    ASSERT_EQ(4, static_cast<int>(s6 / s4));
    ASSERT_EQ(0, static_cast<int>(s4 / s6));
}

TEST(CalcTest, DivTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 / 3).size());
    ASSERT_EQ(4, (3 / s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 / 3));
    ASSERT_EQ(0, static_cast<int>(3 / s4));

    ASSERT_EQ(4, (s4 / 5).size());
    ASSERT_EQ(4, (5 / s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 / 5));
    ASSERT_EQ(1, static_cast<int>(5 / s4));

    ASSERT_EQ(4, (s4 / 3u).size());
    ASSERT_EQ(4, (3u / s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 / 3u));
    ASSERT_EQ(0, static_cast<int>(3u / s4));

    ASSERT_EQ(4, (s4 / 5u).size());
    ASSERT_EQ(4, (5u / s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 / 5u));
    ASSERT_EQ(1, static_cast<int>(5u / s4));
}

TEST(CalcTest, DivTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 / _3).size());
    ASSERT_EQ(4, (_3 / s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 / _3));
    ASSERT_EQ(0, static_cast<int>(_3 / s4));

    ASSERT_EQ(4, (s4 / _5).size());
    ASSERT_EQ(4, (_5 / s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 / _5));
    ASSERT_EQ(1, static_cast<int>(_5 / s4));

    ASSERT_EQ(4, (s4 / _3u).size());
    ASSERT_EQ(4, (_3u / s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 / _3u));
    ASSERT_EQ(0, static_cast<int>(_3u / s4));

    ASSERT_EQ(4, (s4 / _5u).size());
    ASSERT_EQ(4, (_5u / s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 / _5u));
    ASSERT_EQ(1, static_cast<int>(_5u / s4));
}

TEST(CalcTest, Divest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 / 3).size());
    ASSERT_EQ(4, (3 / u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 / 3));
    ASSERT_EQ(0, static_cast<int>(3 / u4));

    ASSERT_EQ(4, (u4 / 5).size());
    ASSERT_EQ(4, (5 / u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 / 5));
    ASSERT_EQ(1, static_cast<int>(5 / u4));

    ASSERT_EQ(4, (u4 / 3u).size());
    ASSERT_EQ(4, (3u / u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 / 3u));
    ASSERT_EQ(0, static_cast<int>(3u / u4));

    ASSERT_EQ(4, (u4 / 5u).size());
    ASSERT_EQ(4, (5u / u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 / 5u));
    ASSERT_EQ(1, static_cast<int>(5u / u4));
}

TEST(CalcTest, DivTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _5(5);
    const Bits<4> _3u(3);
    const Bits<4> _5u(5);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 / _3).size());
    ASSERT_EQ(4, (_3 / u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 / _3));
    ASSERT_EQ(0, static_cast<int>(_3 / u4));

    ASSERT_EQ(4, (u4 / _5).size());
    ASSERT_EQ(4, (_5 / u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 / _5));
    ASSERT_EQ(1, static_cast<int>(_5 / u4));

    ASSERT_EQ(4, (u4 / _3u).size());
    ASSERT_EQ(4, (_3u / u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 / _3u));
    ASSERT_EQ(0, static_cast<int>(_3u / u4));

    ASSERT_EQ(4, (u4 / _5u).size());
    ASSERT_EQ(4, (_5u / u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 / _5u));
    ASSERT_EQ(1, static_cast<int>(_5u / u4));
}

TEST(CalcTest, ModTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 = 10;
    ASSERT_EQ(10, static_cast<int>(s6));
    s6 %= 3;
    ASSERT_EQ(1, static_cast<int>(s6));
    s6 = -10;
    ASSERT_EQ(-10, static_cast<int>(s6));
    s6 %=3;
    ASSERT_EQ(-1, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 = 10;
    ASSERT_EQ(10, static_cast<int>(u6));
    u6 %= 3;
    ASSERT_EQ(1, static_cast<int>(u6));
    u6 = -10;
    ASSERT_EQ(54, static_cast<int>(u6));
    u6 %= 3;
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(CalcTest, ModTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 % s4).size());
    ASSERT_EQ(6, (s4 % s6).size());
    ASSERT_EQ(6, static_cast<int>(s6 % s4));
    ASSERT_EQ(7, static_cast<int>(s4 % s6));

    s6 = 31;
    ASSERT_EQ(3, static_cast<int>(s6 % s4));
    ASSERT_EQ(7, static_cast<int>(s4 % s6));
}

TEST(CalcTest, ModTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 % 3).size());
    ASSERT_EQ(4, (3 % s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 % 3));
    ASSERT_EQ(3, static_cast<int>(3 % s4));

    ASSERT_EQ(4, (s4 % 4).size());
    ASSERT_EQ(4, (4 % s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 % 4));
    ASSERT_EQ(0, static_cast<int>(4 % s4));

    ASSERT_EQ(4, (s4 % 3u).size());
    ASSERT_EQ(4, (3u % s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 % 3u));
    ASSERT_EQ(3, static_cast<int>(3u % s4));

    ASSERT_EQ(4, (s4 % 4u).size());
    ASSERT_EQ(4, (4u % s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 % 4u));
    ASSERT_EQ(0, static_cast<int>(4u % s4));
}

TEST(CalcTest, ModTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _4(4);
    const Bits<4> _3u(3);
    const Bits<4> _4u(4);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 % _3).size());
    ASSERT_EQ(4, (_3 % s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 % _3));
    ASSERT_EQ(3, static_cast<int>(_3 % s4));

    ASSERT_EQ(4, (s4 % _4).size());
    ASSERT_EQ(4, (_4 % s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 % _4));
    ASSERT_EQ(0, static_cast<int>(_4 % s4));

    ASSERT_EQ(4, (s4 % _3u).size());
    ASSERT_EQ(4, (_3u % s4).size());
    ASSERT_EQ(1, static_cast<int>(s4 % _3u));
    ASSERT_EQ(3, static_cast<int>(_3u % s4));

    ASSERT_EQ(4, (s4 % _4u).size());
    ASSERT_EQ(4, (_4u % s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 % _4u));
    ASSERT_EQ(0, static_cast<int>(_4u % s4));
}

TEST(CalcTest, ModTest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 % 3).size());
    ASSERT_EQ(4, (3 % u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 % 3));
    ASSERT_EQ(3, static_cast<int>(3 % u4));

    ASSERT_EQ(4, (u4 % 4).size());
    ASSERT_EQ(4, (4 % u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 % 4));
    ASSERT_EQ(0, static_cast<int>(4 % u4));

    ASSERT_EQ(4, (u4 % 3u).size());
    ASSERT_EQ(4, (3u % u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 % 3u));
    ASSERT_EQ(3, static_cast<int>(3u % u4));

    ASSERT_EQ(4, (u4 % 4u).size());
    ASSERT_EQ(4, (4u % u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 % 4u));
    ASSERT_EQ(0, static_cast<int>(4u % u4));
}

TEST(CalcTest, ModTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _4(4);
    const Bits<4> _3u(3);
    const Bits<4> _4u(4);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 % _3).size());
    ASSERT_EQ(4, (_3 % u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 % _3));
    ASSERT_EQ(3, static_cast<int>(_3 % u4));

    ASSERT_EQ(4, (u4 % _4).size());
    ASSERT_EQ(4, (_4 % u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 % _4));
    ASSERT_EQ(0, static_cast<int>(_4 % u4));

    ASSERT_EQ(4, (u4 % _3u).size());
    ASSERT_EQ(4, (_3u % u4).size());
    ASSERT_EQ(1, static_cast<int>(u4 % _3u));
    ASSERT_EQ(3, static_cast<int>(_3u % u4));

    ASSERT_EQ(4, (u4 % _4u).size());
    ASSERT_EQ(4, (_4u % u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 % _4u));
    ASSERT_EQ(0, static_cast<int>(_4u % u4));
}

TEST(CalcTest, OrTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 |= 8;
    ASSERT_EQ(8, static_cast<int>(s6));
    s6 |= 4;
    ASSERT_EQ(12, static_cast<int>(s6));
    s6 |= 12;
    ASSERT_EQ(12, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 |= 8;
    ASSERT_EQ(8, static_cast<int>(u6));
    u6 |= 4;
    ASSERT_EQ(12, static_cast<int>(u6));
    u6 |= 12;
    ASSERT_EQ(12, static_cast<int>(u6));
}

TEST(CalcTest, OrTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 + s4).size());
    ASSERT_EQ(6, (s4 + s6).size());
    ASSERT_EQ(15, static_cast<int>(s6 | s4));
    ASSERT_EQ(15, static_cast<int>(s4 | s6));

    s6 = 31;
    ASSERT_EQ(31, static_cast<int>(s6 | s4));
    ASSERT_EQ(31, static_cast<int>(s4 | s6));
}

TEST(CalcTest, OrTest3)
{
    Bits<4, signed> s4(4);
    ASSERT_EQ(4, (s4 | 3).size());
    ASSERT_EQ(4, (3 | s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 | 3));
    ASSERT_EQ(7, static_cast<int>(3 | s4));

    ASSERT_EQ(4, (s4 | 12).size());
    ASSERT_EQ(4, (12 | s4).size());
    ASSERT_EQ(-4, static_cast<int>(s4 | 12));
    ASSERT_EQ(-4, static_cast<int>(12 | s4));

    ASSERT_EQ(4, (s4 | 3u).size());
    ASSERT_EQ(4, (3u | s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 | 3u));
    ASSERT_EQ(7, static_cast<int>(3u | s4));

    ASSERT_EQ(4, (s4 | 12u).size());
    ASSERT_EQ(4, (12u | s4).size());
    ASSERT_EQ(12, static_cast<int>(s4 | 12u));
    ASSERT_EQ(12, static_cast<int>(12u | s4));
}

TEST(CalcTest, OrTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4, signed> s4(4);

    ASSERT_EQ(4, (s4 | _3).size());
    ASSERT_EQ(4, (_3 | s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 | _3));
    ASSERT_EQ(7, static_cast<int>(_3 | s4));

    ASSERT_EQ(4, (s4 | _12).size());
    ASSERT_EQ(4, (_12 | s4).size());
    ASSERT_EQ(-4, static_cast<int>(s4 | _12));
    ASSERT_EQ(-4, static_cast<int>(_12 | s4));

    ASSERT_EQ(4, (s4 | _3u).size());
    ASSERT_EQ(4, (_3u | s4).size());
    ASSERT_EQ(7, static_cast<int>(s4 | _3u));
    ASSERT_EQ(7, static_cast<int>(_3u | s4));

    ASSERT_EQ(4, (s4 | _12u).size());
    ASSERT_EQ(4, (_12u | s4).size());
    ASSERT_EQ(12, static_cast<int>(s4 | _12u));
    ASSERT_EQ(12, static_cast<int>(_12u | s4));
}

TEST(CalcTest, OrTest5)
{
    Bits<4> u4(4);
    ASSERT_EQ(4, (u4 | 3).size());
    ASSERT_EQ(4, (3 | u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 | 3));
    ASSERT_EQ(7, static_cast<int>(3 | u4));

    ASSERT_EQ(4, (u4 | -4).size());
    ASSERT_EQ(4, (-4 | u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 | -4));
    ASSERT_EQ(12, static_cast<int>(-4 | u4));

    ASSERT_EQ(4, (u4 | 3u).size());
    ASSERT_EQ(4, (3u | u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 | 3u));
    ASSERT_EQ(7, static_cast<int>(3u | u4));

    ASSERT_EQ(4, (u4 | 12u).size());
    ASSERT_EQ(4, (12u | u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 | 12u));
    ASSERT_EQ(12, static_cast<int>(12u | u4));
}

TEST(CalcTest, OrTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4> u4(4);

    ASSERT_EQ(4, (u4 | _3).size());
    ASSERT_EQ(4, (_3 | u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 | _3));
    ASSERT_EQ(7, static_cast<int>(_3 | u4));

    ASSERT_EQ(4, (u4 | _12).size());
    ASSERT_EQ(4, (_12 | u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 | _12));
    ASSERT_EQ(12, static_cast<int>(_12 | u4));

    ASSERT_EQ(4, (u4 | _3u).size());
    ASSERT_EQ(4, (_3u | u4).size());
    ASSERT_EQ(7, static_cast<int>(u4 | _3u));
    ASSERT_EQ(7, static_cast<int>(_3u | u4));

    ASSERT_EQ(4, (u4 | _12u).size());
    ASSERT_EQ(4, (_12u | u4).size());
    ASSERT_EQ(12, static_cast<int>(u4 | _12u));
    ASSERT_EQ(12, static_cast<int>(_12u | u4));
}

TEST(CalcTest, AndTest1)
{
    Bits<6, signed> s6(-1);
    ASSERT_EQ(-1, static_cast<int>(s6));
    s6 &= 62;
    ASSERT_EQ(-2, static_cast<int>(s6));
    s6 &= 31;
    ASSERT_EQ(30, static_cast<int>(s6));
    s6 &= 7;
    ASSERT_EQ(6, static_cast<int>(s6));

    Bits<6> u6(63);
    ASSERT_EQ(63, static_cast<int>(u6));
    u6 &= 31;
    ASSERT_EQ(31, static_cast<int>(u6));
    u6 &= 7;
    ASSERT_EQ(7, static_cast<int>(u6));
    u6 &= 12;
    ASSERT_EQ(4, static_cast<int>(u6));
}

TEST(CalcTest, AndTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 + s4).size());
    ASSERT_EQ(6, (s4 + s6).size());
    ASSERT_EQ(5, static_cast<int>(s6 & s4));
    ASSERT_EQ(5, static_cast<int>(s4 & s6));

    s6 = 31;
    ASSERT_EQ(7, static_cast<int>(s6 & s4));
    ASSERT_EQ(7, static_cast<int>(s4 & s6));
}

TEST(CalcTest, AndTest3)
{
    Bits<4, signed> s4(8);
    ASSERT_EQ(4, (s4 & 3).size());
    ASSERT_EQ(4, (3 & s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 & 3));
    ASSERT_EQ(0, static_cast<int>(3 & s4));

    ASSERT_EQ(4, (s4 & 12).size());
    ASSERT_EQ(4, (12 & s4).size());
    ASSERT_EQ(-8, static_cast<int>(s4 & 12));
    ASSERT_EQ(-8, static_cast<int>(12 & s4));

    ASSERT_EQ(4, (s4 & 3u).size());
    ASSERT_EQ(4, (3u & s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 & 3u));
    ASSERT_EQ(0, static_cast<int>(3u & s4));

    ASSERT_EQ(4, (s4 & 12u).size());
    ASSERT_EQ(4, (12u & s4).size());
    ASSERT_EQ(8, static_cast<int>(s4 & 12u));
    ASSERT_EQ(8, static_cast<int>(12u & s4));
}

TEST(CalcTest, AndTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4, signed> s4(8);

    ASSERT_EQ(4, (s4 & _3).size());
    ASSERT_EQ(4, (_3 & s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 & _3));
    ASSERT_EQ(0, static_cast<int>(_3 & s4));

    ASSERT_EQ(4, (s4 & _12).size());
    ASSERT_EQ(4, (_12 & s4).size());
    ASSERT_EQ(-8, static_cast<int>(s4 & _12));
    ASSERT_EQ(-8, static_cast<int>(_12 & s4));

    ASSERT_EQ(4, (s4 & _3u).size());
    ASSERT_EQ(4, (_3u & s4).size());
    ASSERT_EQ(0, static_cast<int>(s4 & _3u));
    ASSERT_EQ(0, static_cast<int>(_3u & s4));

    ASSERT_EQ(4, (s4 & _12u).size());
    ASSERT_EQ(4, (_12u & s4).size());
    ASSERT_EQ(8, static_cast<int>(s4 & _12u));
    ASSERT_EQ(8, static_cast<int>(_12u & s4));
}

TEST(CalcTest, AndTest5)
{
    Bits<4> u4(8);
    ASSERT_EQ(4, (u4 & 3).size());
    ASSERT_EQ(4, (3 & u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 & 3));
    ASSERT_EQ(0, static_cast<int>(3 & u4));

    ASSERT_EQ(4, (u4 & -4).size());
    ASSERT_EQ(4, (-4 & u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 & -4));
    ASSERT_EQ(8, static_cast<int>(-4 & u4));

    ASSERT_EQ(4, (u4 & 3u).size());
    ASSERT_EQ(4, (3u & u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 & 3u));
    ASSERT_EQ(0, static_cast<int>(3u & u4));

    ASSERT_EQ(4, (u4 & 12u).size());
    ASSERT_EQ(4, (12u & u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 & 12u));
    ASSERT_EQ(8, static_cast<int>(12u & u4));
}

TEST(CalcTest, AndTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4> u4(8);

    ASSERT_EQ(4, (u4 & _3).size());
    ASSERT_EQ(4, (_3 & u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 & _3));
    ASSERT_EQ(0, static_cast<int>(_3 & u4));

    ASSERT_EQ(4, (u4 & _12).size());
    ASSERT_EQ(4, (_12 & u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 & _12));
    ASSERT_EQ(8, static_cast<int>(_12 & u4));

    ASSERT_EQ(4, (u4 & _3u).size());
    ASSERT_EQ(4, (_3u & u4).size());
    ASSERT_EQ(0, static_cast<int>(u4 & _3u));
    ASSERT_EQ(0, static_cast<int>(_3u & u4));

    ASSERT_EQ(4, (u4 & _12u).size());
    ASSERT_EQ(4, (_12u & u4).size());
    ASSERT_EQ(8, static_cast<int>(u4 & _12u));
    ASSERT_EQ(8, static_cast<int>(_12u & u4));
}

TEST(CalcTest, XorTest1)
{
    Bits<6, signed> s6;
    ASSERT_EQ(0, static_cast<int>(s6));
    s6 ^= 8;
    ASSERT_EQ(8, static_cast<int>(s6));
    s6 ^= 4;
    ASSERT_EQ(12, static_cast<int>(s6));
    s6 ^= 12;
    ASSERT_EQ(0, static_cast<int>(s6));

    Bits<6> u6;
    ASSERT_EQ(0, static_cast<int>(u6));
    u6 ^= 8;
    ASSERT_EQ(8, static_cast<int>(u6));
    u6 ^= 4;
    ASSERT_EQ(12, static_cast<int>(u6));
    u6 ^= 12;
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(CalcTest, XorTest2)
{
    Bits<6, signed> s6(13);
    Bits<4, signed> s4(7);
    ASSERT_EQ(6, (s6 + s4).size());
    ASSERT_EQ(6, (s4 + s6).size());
    ASSERT_EQ(10, static_cast<int>(s6 ^ s4));
    ASSERT_EQ(10, static_cast<int>(s4 ^ s6));

    s6 = 31;
    ASSERT_EQ(24, static_cast<int>(s6 ^ s4));
    ASSERT_EQ(24, static_cast<int>(s4 ^ s6));
}

TEST(CalcTest, XorTest3)
{
    Bits<4, signed> s4(8);
    ASSERT_EQ(4, (s4 ^ 3).size());
    ASSERT_EQ(4, (3 ^ s4).size());
    ASSERT_EQ(-5, static_cast<int>(s4 ^ 3));
    ASSERT_EQ(-5, static_cast<int>(3 ^ s4));

    ASSERT_EQ(4, (s4 ^ 12).size());
    ASSERT_EQ(4, (12 ^ s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 ^ 12));
    ASSERT_EQ(4, static_cast<int>(12 ^ s4));

    ASSERT_EQ(4, (s4 ^ 3u).size());
    ASSERT_EQ(4, (3u ^ s4).size());
    ASSERT_EQ(11, static_cast<int>(s4 ^ 3u));
    ASSERT_EQ(11, static_cast<int>(3u ^ s4));

    ASSERT_EQ(4, (s4 ^ 12u).size());
    ASSERT_EQ(4, (12u ^ s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 ^ 12u));
    ASSERT_EQ(4, static_cast<int>(12u ^ s4));
}

TEST(CalcTest, XorTest4)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4, signed> s4(8);

    ASSERT_EQ(4, (s4 ^ _3).size());
    ASSERT_EQ(4, (_3 ^ s4).size());
    ASSERT_EQ(-5, static_cast<int>(s4 ^ _3));
    ASSERT_EQ(-5, static_cast<int>(_3 ^ s4));

    ASSERT_EQ(4, (s4 ^ _12).size());
    ASSERT_EQ(4, (_12 ^ s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 ^ _12));
    ASSERT_EQ(4, static_cast<int>(_12 ^ s4));

    ASSERT_EQ(4, (s4 ^ _3u).size());
    ASSERT_EQ(4, (_3u ^ s4).size());
    ASSERT_EQ(11, static_cast<int>(s4 ^ _3u));
    ASSERT_EQ(11, static_cast<int>(_3u ^ s4));

    ASSERT_EQ(4, (s4 ^ _12u).size());
    ASSERT_EQ(4, (_12u ^ s4).size());
    ASSERT_EQ(4, static_cast<int>(s4 ^ _12u));
    ASSERT_EQ(4, static_cast<int>(_12u ^ s4));
}

TEST(CalcTest, XorTest5)
{
    Bits<4> u4(8);
    ASSERT_EQ(4, (u4 ^ 3).size());
    ASSERT_EQ(4, (3 ^ u4).size());
    ASSERT_EQ(11, static_cast<int>(u4 ^ 3));
    ASSERT_EQ(11, static_cast<int>(3 ^ u4));

    ASSERT_EQ(4, (u4 ^ -4).size());
    ASSERT_EQ(4, (-4 ^ u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 ^ -4));
    ASSERT_EQ(4, static_cast<int>(-4 ^ u4));

    ASSERT_EQ(4, (u4 ^ 3u).size());
    ASSERT_EQ(4, (3u ^ u4).size());
    ASSERT_EQ(11, static_cast<int>(u4 ^ 3u));
    ASSERT_EQ(11, static_cast<int>(3u ^ u4));

    ASSERT_EQ(4, (u4 ^ 12u).size());
    ASSERT_EQ(4, (12u ^ u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 ^ 12u));
    ASSERT_EQ(4, static_cast<int>(12u ^ u4));
}

TEST(CalcTest, XorTest6)
{
    const Bits<4, signed> _3(3);
    const Bits<4, signed> _12(12);
    const Bits<4> _3u(3);
    const Bits<4> _12u(12);

    Bits<4> u4(8);

    ASSERT_EQ(4, (u4 ^ _3).size());
    ASSERT_EQ(4, (_3 ^ u4).size());
    ASSERT_EQ(11, static_cast<int>(u4 ^ _3));
    ASSERT_EQ(11, static_cast<int>(_3 ^ u4));

    ASSERT_EQ(4, (u4 ^ _12).size());
    ASSERT_EQ(4, (_12 ^ u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 ^ _12));
    ASSERT_EQ(4, static_cast<int>(_12 ^ u4));

    ASSERT_EQ(4, (u4 ^ _3u).size());
    ASSERT_EQ(4, (_3u ^ u4).size());
    ASSERT_EQ(11, static_cast<int>(u4 ^ _3u));
    ASSERT_EQ(11, static_cast<int>(_3u ^ u4));

    ASSERT_EQ(4, (u4 ^ _12u).size());
    ASSERT_EQ(4, (_12u ^ u4).size());
    ASSERT_EQ(4, static_cast<int>(u4 ^ _12u));
    ASSERT_EQ(4, static_cast<int>(_12u ^ u4));
}

TEST(CalcTest, SizeTest)
{
    Bits<4> u4(1);
    Bits<6> u6(1);

    ASSERT_EQ(4, (u4 + u4).size());
    ASSERT_EQ(4, (u4 - u4).size());
    ASSERT_EQ(4, (u4 * u4).size());
    ASSERT_EQ(4, (u4 / u4).size());
    ASSERT_EQ(4, (u4 % u4).size());
    ASSERT_EQ(4, (u4 | u4).size());
    ASSERT_EQ(4, (u4 & u4).size());
    ASSERT_EQ(4, (u4 ^ u4).size());

    ASSERT_EQ(6, (u4 + u6).size());
    ASSERT_EQ(6, (u4 - u6).size());
    ASSERT_EQ(6, (u4 * u6).size());
    ASSERT_EQ(6, (u4 / u6).size());
    ASSERT_EQ(6, (u4 % u6).size());
    ASSERT_EQ(6, (u4 | u6).size());
    ASSERT_EQ(6, (u4 & u6).size());
    ASSERT_EQ(6, (u4 ^ u6).size());

    ASSERT_EQ(6, (u6 + u4).size());
    ASSERT_EQ(6, (u6 - u4).size());
    ASSERT_EQ(6, (u6 * u4).size());
    ASSERT_EQ(6, (u6 / u4).size());
    ASSERT_EQ(6, (u6 % u4).size());
    ASSERT_EQ(6, (u6 | u4).size());
    ASSERT_EQ(6, (u6 & u4).size());
    ASSERT_EQ(6, (u6 ^ u4).size());

    ASSERT_EQ(6, (u6 + u6).size());
    ASSERT_EQ(6, (u6 - u6).size());
    ASSERT_EQ(6, (u6 * u6).size());
    ASSERT_EQ(6, (u6 / u6).size());
    ASSERT_EQ(6, (u6 % u6).size());
    ASSERT_EQ(6, (u6 | u6).size());
    ASSERT_EQ(6, (u6 & u6).size());
    ASSERT_EQ(6, (u6 ^ u6).size());


    Bits<4, signed> s4(1);
    Bits<6, signed> s6(1);

    ASSERT_EQ(4, (s4 + s4).size());
    ASSERT_EQ(4, (s4 - s4).size());
    ASSERT_EQ(4, (s4 * s4).size());
    ASSERT_EQ(4, (s4 / s4).size());
    ASSERT_EQ(4, (s4 % s4).size());
    ASSERT_EQ(4, (s4 | s4).size());
    ASSERT_EQ(4, (s4 & s4).size());
    ASSERT_EQ(4, (s4 ^ s4).size());

    ASSERT_EQ(6, (s4 + s6).size());
    ASSERT_EQ(6, (s4 - s6).size());
    ASSERT_EQ(6, (s4 * s6).size());
    ASSERT_EQ(6, (s4 / s6).size());
    ASSERT_EQ(6, (s4 % s6).size());
    ASSERT_EQ(6, (s4 | s6).size());
    ASSERT_EQ(6, (s4 & s6).size());
    ASSERT_EQ(6, (s4 ^ s6).size());

    ASSERT_EQ(6, (s6 + s4).size());
    ASSERT_EQ(6, (s6 - s4).size());
    ASSERT_EQ(6, (s6 * s4).size());
    ASSERT_EQ(6, (s6 / s4).size());
    ASSERT_EQ(6, (s6 % s4).size());
    ASSERT_EQ(6, (s6 | s4).size());
    ASSERT_EQ(6, (s6 & s4).size());
    ASSERT_EQ(6, (s6 ^ s4).size());

    ASSERT_EQ(6, (s6 + s6).size());
    ASSERT_EQ(6, (s6 - s6).size());
    ASSERT_EQ(6, (s6 * s6).size());
    ASSERT_EQ(6, (s6 / s6).size());
    ASSERT_EQ(6, (s6 % s6).size());
    ASSERT_EQ(6, (s6 | s6).size());
    ASSERT_EQ(6, (s6 & s6).size());
    ASSERT_EQ(6, (s6 ^ s6).size());


    ASSERT_EQ(4, (u4 + s4).size());
    ASSERT_EQ(4, (u4 - s4).size());
    ASSERT_EQ(4, (u4 * s4).size());
    ASSERT_EQ(4, (u4 / s4).size());
    ASSERT_EQ(4, (u4 % s4).size());
    ASSERT_EQ(4, (u4 | s4).size());
    ASSERT_EQ(4, (u4 & s4).size());
    ASSERT_EQ(4, (u4 ^ s4).size());

    ASSERT_EQ(6, (u4 + s6).size());
    ASSERT_EQ(6, (u4 - s6).size());
    ASSERT_EQ(6, (u4 * s6).size());
    ASSERT_EQ(6, (u4 / s6).size());
    ASSERT_EQ(6, (u4 % s6).size());
    ASSERT_EQ(6, (u4 | s6).size());
    ASSERT_EQ(6, (u4 & s6).size());
    ASSERT_EQ(6, (u4 ^ s6).size());

    ASSERT_EQ(6, (u6 + s4).size());
    ASSERT_EQ(6, (u6 - s4).size());
    ASSERT_EQ(6, (u6 * s4).size());
    ASSERT_EQ(6, (u6 / s4).size());
    ASSERT_EQ(6, (u6 % s4).size());
    ASSERT_EQ(6, (u6 | s4).size());
    ASSERT_EQ(6, (u6 & s4).size());
    ASSERT_EQ(6, (u6 ^ s4).size());

    ASSERT_EQ(6, (u6 + s6).size());
    ASSERT_EQ(6, (u6 - s6).size());
    ASSERT_EQ(6, (u6 * s6).size());
    ASSERT_EQ(6, (u6 / s6).size());
    ASSERT_EQ(6, (u6 % s6).size());
    ASSERT_EQ(6, (u6 | s6).size());
    ASSERT_EQ(6, (u6 & s6).size());
    ASSERT_EQ(6, (u6 ^ s6).size());


    ASSERT_EQ(4, (s4 + u4).size());
    ASSERT_EQ(4, (s4 - u4).size());
    ASSERT_EQ(4, (s4 * u4).size());
    ASSERT_EQ(4, (s4 / u4).size());
    ASSERT_EQ(4, (s4 % u4).size());
    ASSERT_EQ(4, (s4 | u4).size());
    ASSERT_EQ(4, (s4 & u4).size());
    ASSERT_EQ(4, (s4 ^ u4).size());

    ASSERT_EQ(6, (s4 + u6).size());
    ASSERT_EQ(6, (s4 - u6).size());
    ASSERT_EQ(6, (s4 * u6).size());
    ASSERT_EQ(6, (s4 / u6).size());
    ASSERT_EQ(6, (s4 % u6).size());
    ASSERT_EQ(6, (s4 | u6).size());
    ASSERT_EQ(6, (s4 & u6).size());
    ASSERT_EQ(6, (s4 ^ u6).size());

    ASSERT_EQ(6, (s6 + u4).size());
    ASSERT_EQ(6, (s6 - u4).size());
    ASSERT_EQ(6, (s6 * u4).size());
    ASSERT_EQ(6, (s6 / u4).size());
    ASSERT_EQ(6, (s6 % u4).size());
    ASSERT_EQ(6, (s6 | u4).size());
    ASSERT_EQ(6, (s6 & u4).size());
    ASSERT_EQ(6, (s6 ^ u4).size());

    ASSERT_EQ(6, (s6 + u6).size());
    ASSERT_EQ(6, (s6 - u6).size());
    ASSERT_EQ(6, (s6 * u6).size());
    ASSERT_EQ(6, (s6 / u6).size());
    ASSERT_EQ(6, (s6 % u6).size());
    ASSERT_EQ(6, (s6 | u6).size());
    ASSERT_EQ(6, (s6 & u6).size());
    ASSERT_EQ(6, (s6 ^ u6).size());
}

TEST(ShiftTest, SignedLeftShiftTest)
{
    Bits<6, signed> s6(31);
    ASSERT_EQ(31, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 << 1).size());
    ASSERT_EQ(-2, static_cast<int>(s6 << 1));
    s6 <<= 1;
    ASSERT_EQ(-2, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 << 2).size());
    ASSERT_EQ(-8, static_cast<int>(s6 << 2));
    s6 <<= 2;
    ASSERT_EQ(-8, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 << 3).size());
    ASSERT_EQ(0, static_cast<int>(s6 << 3));
    s6 <<= 3;
    ASSERT_EQ(0, static_cast<int>(s6));
}

TEST(ShiftTest, SignedRightShiftTest1)
{
    Bits<6, signed> s6(31);
    ASSERT_EQ(31, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 1).size());
    ASSERT_EQ(15, static_cast<int>(s6 >> 1));
    s6 >>= 1;
    ASSERT_EQ(15, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 2).size());
    ASSERT_EQ(3, static_cast<int>(s6 >> 2));
    s6 >>= 2;
    ASSERT_EQ(3, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 3).size());
    ASSERT_EQ(0, static_cast<int>(s6 >> 3));
    s6 >>= 3;
    ASSERT_EQ(0, static_cast<int>(s6));
}

TEST(ShiftTest, SignedRightShiftTest2)
{
    Bits<6, signed> s6(-32);
    ASSERT_EQ(-32, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 1).size());
    ASSERT_EQ(-16, static_cast<int>(s6 >> 1));
    s6 >>= 1;
    ASSERT_EQ(-16, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 2).size());
    ASSERT_EQ(-4, static_cast<int>(s6 >> 2));
    s6 >>= 2;
    ASSERT_EQ(-4, static_cast<int>(s6));

    ASSERT_EQ(6, (s6 >> 3).size());
    ASSERT_EQ(-1, static_cast<int>(s6 >> 3));
    s6 >>= 3;
    ASSERT_EQ(-1, static_cast<int>(s6));
}

TEST(ShiftTest, UnsignedLeftShiftTest)
{
    Bits<6> u6(31);
    ASSERT_EQ(31, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 << 1).size());
    ASSERT_EQ(62, static_cast<int>(u6 << 1));
    u6 <<= 1;
    ASSERT_EQ(62, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 << 2).size());
    ASSERT_EQ(56, static_cast<int>(u6 << 2));
    u6 <<= 2;
    ASSERT_EQ(56, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 << 3).size());
    ASSERT_EQ(0, static_cast<int>(u6 << 3));
    u6 <<= 3;
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(ShiftTest, UnsignedRightShiftTest1)
{
    Bits<6> u6(31);
    ASSERT_EQ(31, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 1).size());
    ASSERT_EQ(15, static_cast<int>(u6 >> 1));
    u6 >>= 1;
    ASSERT_EQ(15, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 2).size());
    ASSERT_EQ(3, static_cast<int>(u6 >> 2));
    u6 >>= 2;
    ASSERT_EQ(3, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 3).size());
    ASSERT_EQ(0, static_cast<int>(u6 >> 3));
    u6 >>= 3;
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(ShiftTest, UnsignedRightShiftTest2)
{
    Bits<6> u6(32);
    ASSERT_EQ(32, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 1).size());
    ASSERT_EQ(16, static_cast<int>(u6 >> 1));
    u6 >>= 1;
    ASSERT_EQ(16, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 2).size());
    ASSERT_EQ(4, static_cast<int>(u6 >> 2));
    u6 >>= 2;
    ASSERT_EQ(4, static_cast<int>(u6));

    ASSERT_EQ(6, (u6 >> 3).size());
    ASSERT_EQ(0, static_cast<int>(u6 >> 3));
    u6 >>= 3;
    ASSERT_EQ(0, static_cast<int>(u6));
}

TEST(AssignmentTest, SameSizeTest1)
{
    Bits<8> a(0x12);
    Bits<8> b(0x34);

    ASSERT_EQ(0x12, static_cast<int>(a));
    ASSERT_EQ(0x34, static_cast<int>(b));

    (a, b) = (b, a);

    ASSERT_EQ(0x34, static_cast<int>(a));
    ASSERT_EQ(0x12, static_cast<int>(b));

    Bits<8, signed> c(0x12);
    Bits<8, signed> d(0x34);

    ASSERT_EQ(0x12, static_cast<int>(c));
    ASSERT_EQ(0x34, static_cast<int>(d));

    (c, d) = (d, c);

    ASSERT_EQ(0x34, static_cast<int>(c));
    ASSERT_EQ(0x12, static_cast<int>(d));
}

TEST(AssignmentTest, SameSizeTest2)
{
    Bits<8> a(0x12);
    Bits<8> b(0x34);
    Bits<8> c(0x56);

    ASSERT_EQ(0x12, static_cast<int>(a));
    ASSERT_EQ(0x34, static_cast<int>(b));
    ASSERT_EQ(0x56, static_cast<int>(c));

    (a, b, c) = (b, c, a);

    ASSERT_EQ(0x34, static_cast<int>(a));
    ASSERT_EQ(0x56, static_cast<int>(b));
    ASSERT_EQ(0x12, static_cast<int>(c));
}

TEST(AssignmentTest, DifferentSizeTest1)
{
    Bits<4>  a(0x1);
    Bits<12> b(0x234);

    ASSERT_EQ(0x1, static_cast<int>(a));
    ASSERT_EQ(0x234, static_cast<int>(b));

    (a, b) = (b, a);

    ASSERT_EQ(0x2, static_cast<int>(a));
    ASSERT_EQ(0x341, static_cast<int>(b));

    Bits<4,  signed> c(0x1);
    Bits<12, signed> d(0x234);

    ASSERT_EQ(0x1, static_cast<int>(c));
    ASSERT_EQ(0x234, static_cast<int>(d));

    (c, d) = (d, c);

    ASSERT_EQ(0x2, static_cast<int>(c));
    ASSERT_EQ(0x341, static_cast<int>(d));
}

TEST(AssignmentTest, DifferentSizeTest2)
{
    Bits<4>  a(0x1);
    Bits<8>  b(0x23);
    Bits<12> c(0x456);

    ASSERT_EQ(0x1, static_cast<int>(a));
    ASSERT_EQ(0x23, static_cast<int>(b));
    ASSERT_EQ(0x456, static_cast<int>(c));

    (a, b, c) = (b, c, a);

    ASSERT_EQ(0x2, static_cast<int>(a));
    ASSERT_EQ(0x34, static_cast<int>(b));
    ASSERT_EQ(0x561, static_cast<int>(c));
}

TEST(AssignmentTest, DifferentSizeTest3)
{
    const Bits<4>  a(0x1);
    const Bits<8>  b(0x23);
    const Bits<12> c(0x456);

    ASSERT_EQ(0x1, static_cast<int>(a));
    ASSERT_EQ(0x23, static_cast<int>(b));
    ASSERT_EQ(0x456, static_cast<int>(c));

    Bits<4>  d;
    Bits<8>  e;
    Bits<12> f;

    (d, e, f) = (b, c, a);

    ASSERT_EQ(0x2, static_cast<int>(d));
    ASSERT_EQ(0x34, static_cast<int>(e));
    ASSERT_EQ(0x561, static_cast<int>(f));
}

TEST(SignTest, SignedTest)
{
    Bits<4, signed> s4(7);

    ASSERT_EQ(7, static_cast<int>(s4));
    ASSERT_EQ(4, (+s4).size());
    ASSERT_EQ(4, (-s4).size());
    ASSERT_EQ(7, static_cast<int>(+s4));
    ASSERT_EQ(-7, static_cast<int>(-s4));

    s4 = -8;
    ASSERT_EQ(-8, static_cast<int>(s4));
    ASSERT_EQ(4, (+s4).size());
    ASSERT_EQ(4, (-s4).size());
    ASSERT_EQ(-8, static_cast<int>(+s4));
    ASSERT_EQ(-8, static_cast<int>(-s4));
}

TEST(SignTest, UnsignedTest)
{
    Bits<4> u4(7);

    ASSERT_EQ(7, static_cast<int>(u4));
    ASSERT_EQ(4, (+u4).size());
    ASSERT_EQ(4, (-u4).size());
    ASSERT_EQ(7, static_cast<int>(+u4));
    ASSERT_EQ(9, static_cast<int>(-u4));

    u4 = 8;
    ASSERT_EQ(8, static_cast<int>(u4));
    ASSERT_EQ(4, (+u4).size());
    ASSERT_EQ(4, (-u4).size());
    ASSERT_EQ(8, static_cast<int>(+u4));
    ASSERT_EQ(8, static_cast<int>(-u4));
}

TEST(NotTest, SignedTest)
{
    Bits<4, signed> s4(7);

    ASSERT_EQ(7, static_cast<int>(s4));
    ASSERT_EQ(4, (~s4).size());
    ASSERT_EQ(-8, static_cast<int>(~s4));

    s4 = -8;
    ASSERT_EQ(-8, static_cast<int>(s4));
    ASSERT_EQ(4, (~s4).size());
    ASSERT_EQ(7, static_cast<int>(~s4));
}

TEST(NotTest, UnsignedTest)
{
    Bits<4> u4(7);

    ASSERT_EQ(7, static_cast<int>(u4));
    ASSERT_EQ(4, (~u4).size());
    ASSERT_EQ(8, static_cast<int>(~u4));

    u4 = 8;
    ASSERT_EQ(8, static_cast<int>(u4));
    ASSERT_EQ(4, (~u4).size());
    ASSERT_EQ(7, static_cast<int>(~u4));
}

TEST(IncTest, SignedTest)
{
    Bits<3, signed> s3;
    ASSERT_EQ(1, ++s3);
    ASSERT_EQ(2, ++s3);
    ASSERT_EQ(3, ++s3);
    ASSERT_EQ(-4, ++s3);
    ASSERT_EQ(-3, ++s3);
    ASSERT_EQ(-2, ++s3);
    ASSERT_EQ(-1, ++s3);
    ASSERT_EQ(0, ++s3);

    s3 = 0;
    ASSERT_EQ(0, s3++);
    ASSERT_EQ(1, s3++);
    ASSERT_EQ(2, s3++);
    ASSERT_EQ(3, s3++);
    ASSERT_EQ(-4, s3++);
    ASSERT_EQ(-3, s3++);
    ASSERT_EQ(-2, s3++);
    ASSERT_EQ(-1, s3++);
    ASSERT_EQ(0, s3);
}

TEST(IncTest, UnsignedTest)
{
    Bits<3> u3;
    ASSERT_EQ(1, ++u3);
    ASSERT_EQ(2, ++u3);
    ASSERT_EQ(3, ++u3);
    ASSERT_EQ(4, ++u3);
    ASSERT_EQ(5, ++u3);
    ASSERT_EQ(6, ++u3);
    ASSERT_EQ(7, ++u3);
    ASSERT_EQ(0, ++u3);

    u3 = 0;
    ASSERT_EQ(0, u3++);
    ASSERT_EQ(1, u3++);
    ASSERT_EQ(2, u3++);
    ASSERT_EQ(3, u3++);
    ASSERT_EQ(4, u3++);
    ASSERT_EQ(5, u3++);
    ASSERT_EQ(6, u3++);
    ASSERT_EQ(7, u3++);
    ASSERT_EQ(0, u3);
}

TEST(DecTest, SignedTests)
{
    Bits<3, signed> s3;
    ASSERT_EQ(-1, --s3);
    ASSERT_EQ(-2, --s3);
    ASSERT_EQ(-3, --s3);
    ASSERT_EQ(-4, --s3);
    ASSERT_EQ(3, --s3);
    ASSERT_EQ(2, --s3);
    ASSERT_EQ(1, --s3);
    ASSERT_EQ(0, --s3);

    s3 = 0;
    ASSERT_EQ(0, s3--);
    ASSERT_EQ(-1, s3--);
    ASSERT_EQ(-2, s3--);
    ASSERT_EQ(-3, s3--);
    ASSERT_EQ(-4, s3--);
    ASSERT_EQ(3, s3--);
    ASSERT_EQ(2, s3--);
    ASSERT_EQ(1, s3--);
    ASSERT_EQ(0, s3--);
}

TEST(DecTest, UnsignedTest)
{
    Bits<3> u3;
    ASSERT_EQ(7, --u3);
    ASSERT_EQ(6, --u3);
    ASSERT_EQ(5, --u3);
    ASSERT_EQ(4, --u3);
    ASSERT_EQ(3, --u3);
    ASSERT_EQ(2, --u3);
    ASSERT_EQ(1, --u3);
    ASSERT_EQ(0, --u3);

    u3 = 0;
    ASSERT_EQ(0, u3--);
    ASSERT_EQ(7, u3--);
    ASSERT_EQ(6, u3--);
    ASSERT_EQ(5, u3--);
    ASSERT_EQ(4, u3--);
    ASSERT_EQ(3, u3--);
    ASSERT_EQ(2, u3--);
    ASSERT_EQ(1, u3--);
    ASSERT_EQ(0, u3--);
}

TEST(DefineSignTest, SignedTest)
{
    ASSERT_EQ(typeid(signed char),  typeid(Bits<1, Signed>::value_type));
    ASSERT_EQ(typeid(signed char),  typeid(Bits<8, Signed>::value_type));
    ASSERT_EQ(typeid(signed short), typeid(Bits<9, Signed>::value_type));
    ASSERT_EQ(typeid(signed short), typeid(Bits<16, Signed>::value_type));
    ASSERT_EQ(typeid(signed int),   typeid(Bits<17, Signed>::value_type));
    ASSERT_EQ(typeid(signed int),   typeid(Bits<32, Signed>::value_type));
}

TEST(DefineSignTest, UnsignedTest)
{
    ASSERT_EQ(typeid(unsigned char),  typeid(Bits<1, Unsigned>::value_type));
    ASSERT_EQ(typeid(unsigned char),  typeid(Bits<8, Unsigned>::value_type));
    ASSERT_EQ(typeid(unsigned short), typeid(Bits<9, Unsigned>::value_type));
    ASSERT_EQ(typeid(unsigned short), typeid(Bits<16, Unsigned>::value_type));
    ASSERT_EQ(typeid(unsigned int),   typeid(Bits<17, Unsigned>::value_type));
    ASSERT_EQ(typeid(unsigned int),   typeid(Bits<32, Unsigned>::value_type));
}

TEST(MultiByteTest, DefineTest)
{
    Bits<33>     u33;
    Bits<123>    u123;
    Bits<123456> u123456;

    ASSERT_EQ(5,     sizeof(u33));
    ASSERT_EQ(16,    sizeof(u123));
    ASSERT_EQ(15432, sizeof(u123456));
}

// 汚染テスト：operator , が他の型に影響を与えないこと
struct Foo {};
TEST(ContaminationTest, Test1)
{
    Foo a;
    Foo b;
    Foo c;
    
    c = (a, b);
}

// entry point
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}