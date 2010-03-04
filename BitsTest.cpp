// compile: g++ -Wall -o BitsTest BitsTest.cpp -lgtest
// need Google Test (see: http://code.google.com/p/googletest/ )

#include <gtest/gtest.h>

#include "Bits.h"

using namespace emattsan;

// 各々のビット列のサイズが設定された値になっていること
TEST(SizeTest, Test0)
{
    bits::Signed<1> sbits1;
    ASSERT_EQ(1, sbits1.size());

    bits::Signed<4> sbits4;
    ASSERT_EQ(4, sbits4.size());

    bits::Unsigned<1> ubits1;
    ASSERT_EQ(1, ubits1.size());

    bits::Unsigned<4> ubits4;
    ASSERT_EQ(4, ubits4.size());
}

// 符号付きビット列で初期化ができること
TEST(SignedBitsTest, InitTest)
{
    bits::Signed<4> bits1;
    ASSERT_EQ(0, bits1.get());

    bits::Signed<4> bits2(7);
    ASSERT_EQ(7, bits2.get());

    bits::Signed<4> bits3(-1);
    ASSERT_EQ(-1, bits3.get());

    bits::Signed<4> bits4(-8);
    ASSERT_EQ(-8, bits4.get());

    // 符号付き4bitは格納できる値の範囲は-8〜7なので、8で初期化するとサイクリックして-8になる
    bits::Signed<4> bits5(8);
    ASSERT_EQ(-8, bits5.get()); // cycled
}

// 符号付きビット列で値を設定できること
TEST(SignedBitsTest, SetTest)
{
    bits::Signed<3> bits;
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
    bits::Signed<3> bits;
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
    bits::Unsigned<4> bits1;
    ASSERT_EQ(0u, bits1.get());

    bits::Unsigned<4> bits2(1u);
    ASSERT_EQ(1u, bits2.get());

    bits::Unsigned<4> bits3(7u);
    ASSERT_EQ(7u, bits3.get());

    bits::Unsigned<4> bits4(8u);
    ASSERT_EQ(8u, bits4.get());

    bits::Unsigned<4> bits5(15u);
    ASSERT_EQ(15u, bits5.get());
}

// 符号なしビット列で初期化ができること（サイクリックする場合）
TEST(UnsignedBitsTest, InitTest2)
{
    bits::Unsigned<4> bits1(16u);
    ASSERT_EQ(0u, bits1.get()); // cycled

    bits::Unsigned<4> bits2(17u);
    ASSERT_EQ(1u, bits2.get()); // cycled

    bits::Unsigned<4> bits3(23u);
    ASSERT_EQ(7u, bits3.get()); // cycled

    bits::Unsigned<4> bits4(24u);
    ASSERT_EQ(8u, bits4.get()); // cycled

    bits::Unsigned<4> bits5(31u);
    ASSERT_EQ(15u, bits5.get()); // cycled
}

// 符号なしビット列で値を設定できること
TEST(UnsignedBitsTest, SetTest)
{
    bits::Unsigned<3> bits;
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
    bits::Unsigned<3> bits;
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
    bits::Signed<4> sbits4;
    bits::Signed<3> sbits3;

    ASSERT_EQ(7, (sbits4, sbits3).size());
}

// 連結したビット列から値を得られること
TEST(PackTest, GetTest)
{
    bits::Signed<4> sbits4;
    bits::Signed<3> sbits3;

    sbits4 = 1;
    sbits3 = 2;

    ASSERT_EQ(10, static_cast<int>(sbits4, sbits3));
    ASSERT_EQ(33, static_cast<int>(sbits3, sbits4));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（２つのビット列の連結）
TEST(PackTest, SetTest1)
{
    bits::Signed<4> sbits4;
    bits::Signed<3> sbits3;

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
    bits::Unsigned<2> ubits1;
    bits::Unsigned<3> ubits2;
    bits::Unsigned<3> ubits3;

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
    const bits::Signed<4> highnibble(5);
    const bits::Signed<4> lownibble(10);

    int n = (highnibble, lownibble);

    ASSERT_EQ(0x5a, n);
}

// 連結したビット列から値を得られること（定数と変数が混在したビット列の連結）
TEST(PackTest, MixedGetTest1)
{
    bits::Signed<2>       bits1(1);
    bits::Signed<2>       bits2(1);
    const bits::Signed<2> bits3(1);

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
    bits::Signed<4>   sbits1(1);
    bits::Signed<4>   sbits2(1);
    bits::Unsigned<4> ubits1(1);
    bits::Unsigned<4> ubits2(1);

    ASSERT_EQ(0x1111, static_cast<int>(sbits1, sbits2, ubits1, ubits2));
}

// 連結したビット列から値を得られること（異なるブロックサイズ（内部でビット列を格納する型）が混在したビット列の連結）
TEST(PackTest, MixedGetTest3)
{
    bits::Signed<2, char> sbits1(1);
    bits::Signed<6, int>  sbits2(1);

    ASSERT_EQ(0x41, static_cast<int>(sbits1, sbits2));

    bits::Signed<12, short> sbits3(1);

    ASSERT_EQ(0x41001, static_cast<int>(sbits1, sbits2, sbits3));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（符号付きと符号なしが混在したビット列の連結）
TEST(PackTest, MixedSetTest)
{
    bits::Signed<4>   sbits1;
    bits::Signed<4>   sbits2;
    bits::Unsigned<4> ubits1;
    bits::Unsigned<4> ubits2;

    (sbits1, sbits2, ubits1, ubits2) = 0x89ab;

    ASSERT_EQ(-8, static_cast<int>(sbits1));
    ASSERT_EQ(-7, static_cast<int>(sbits2));
    ASSERT_EQ(10u, static_cast<unsigned int>(ubits1));
    ASSERT_EQ(11u, static_cast<unsigned int>(ubits2));
}

// 連結したビット列から値を得られること（利用しない領域を含む）
TEST(ReservedBitsTest, GetTest)
{
    bits::Signed<3> bits1(2);
    bits::Signed<3> bits2(3);

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));

    // +----+----+----+----+----+----+----+----+
    // |     bits1    |reserved |     bits2    |
    // +----+----+----+----+----+----+----+----+

    ASSERT_EQ(0x43, static_cast<int>(bits1, bits::reserve<2>, bits2));
}

// 連結したビット列に値を代入すると、元のビット列に値が反映されること（利用しない領域を含む）
TEST(ReservedBitsTest, SetTest)
{
    bits::Signed<3> bits1;
    bits::Signed<3> bits2;

    ASSERT_EQ(0, static_cast<int>(bits1));
    ASSERT_EQ(0, static_cast<int>(bits2));

    (bits1, bits::reserve<2>, bits2) = 0x43;

    ASSERT_EQ(2, static_cast<int>(bits1));
    ASSERT_EQ(3, static_cast<int>(bits2));
}

// ビット列型のサイズが値を格納する整数型のサイズと等しいこと
TEST(SizeTest, Test1)
{
    ASSERT_EQ(sizeof(char),  sizeof(bits::Signed<1, char>));
    ASSERT_EQ(sizeof(short), sizeof(bits::Signed<1, short>));
    ASSERT_EQ(sizeof(int),   sizeof(bits::Signed<1, int>));
    ASSERT_EQ(sizeof(long),  sizeof(bits::Signed<1, long>));
    ASSERT_EQ(sizeof(char),  sizeof(bits::Unsigned<1, char>));
    ASSERT_EQ(sizeof(short), sizeof(bits::Unsigned<1, short>));
    ASSERT_EQ(sizeof(int),   sizeof(bits::Unsigned<1, int>));
    ASSERT_EQ(sizeof(long),  sizeof(bits::Unsigned<1, long>));
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（１）
TEST(SizeTest, Test2)
{
    bits::Signed<4, char> bits1;

    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(int), sizeof(bits1, bits1, bits1, bits1, bits1).getSequence());
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（２）
TEST(SizeTest, Test3)
{
    bits::Signed<2, char> bits1;

    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(char), sizeof(bits1, bits1, bits1, bits1).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits1, bits1, bits1, bits1).getSequence());
}

// 連結したビット列の値の型が、その値を格納できる最小の整数型になっていること（３）
TEST(SizeTest, Test4)
{
    bits::Signed<2, char> bits1;

    ASSERT_EQ(sizeof(char), sizeof(bits1, bits::reserve<6>).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits::reserve<7>).getSequence());
    ASSERT_EQ(sizeof(short), sizeof(bits1, bits::reserve<14>).getSequence());
    ASSERT_EQ(sizeof(int), sizeof(bits1, bits::reserve<15>).getSequence());
    ASSERT_EQ(sizeof(int), sizeof(bits1, bits::reserve<30>).getSequence());
}

// entry point
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}