#include <gtest/gtest.h>

#include "Bits.h"

TEST(Bits, SizeTest)
{
    SignedBits<1> sbits1;
    ASSERT_EQ(1, sbits1.size());

    SignedBits<4> sbits4;
    ASSERT_EQ(4, sbits4.size());

    UnsignedBits<1> ubits1;
    ASSERT_EQ(1, ubits1.size());

    UnsignedBits<4> ubits4;
    ASSERT_EQ(4, ubits4.size());
}

TEST(SignedBitsTest, InitTest)
{
    SignedBits<4> bits1;
    ASSERT_EQ(0, bits1.get());

    SignedBits<4> bits2(7);
    ASSERT_EQ(7, bits2.get());

    SignedBits<4> bits3(-1);
    ASSERT_EQ(-1, bits3.get());

    SignedBits<4> bits4(-8);
    ASSERT_EQ(-8, bits4.get());

    SignedBits<4> bits5(8);
    ASSERT_EQ(-8, bits5.get());
}

TEST(SignedBitsTest, SetTest)
{
    SignedBits<3> bits;
    ASSERT_EQ(0, bits.get());

    bits.set(1);
    ASSERT_EQ(1, bits.get());

    bits.set(2);
    ASSERT_EQ(2, bits.get());

    bits.set(3);
    ASSERT_EQ(3, bits.get());

    bits.set(4);
    ASSERT_EQ(-4, bits.get());

    bits.set(5);
    ASSERT_EQ(-3, bits.get());

    bits.set(-3);
    ASSERT_EQ(-3, bits.get());

    bits.set(-2);
    ASSERT_EQ(-2, bits.get());

    bits.set(-1);
    ASSERT_EQ(-1, bits.get());

    bits.set(0);
    ASSERT_EQ(0, bits.get());
}

TEST(UnsignedBitsTest, InitTest1)
{
    UnsignedBits<4> bits1;
    ASSERT_EQ(0u, bits1.get());

    UnsignedBits<4> bits2(1u);
    ASSERT_EQ(1u, bits2.get());

    UnsignedBits<4> bits3(7u);
    ASSERT_EQ(7u, bits3.get());

    UnsignedBits<4> bits4(8u);
    ASSERT_EQ(8u, bits4.get());

    UnsignedBits<4> bits5(15u);
    ASSERT_EQ(15u, bits5.get());
}

TEST(UnsignedBitsTest, InitTest2)
{
    UnsignedBits<4> bits1(16u);
    ASSERT_EQ(0u, bits1.get());

    UnsignedBits<4> bits2(17u);
    ASSERT_EQ(1u, bits2.get());

    UnsignedBits<4> bits3(23u);
    ASSERT_EQ(7u, bits3.get());

    UnsignedBits<4> bits4(24u);
    ASSERT_EQ(8u, bits4.get());

    UnsignedBits<4> bits5(31u);
    ASSERT_EQ(15u, bits5.get());
}

TEST(UnsignedBitsTest, SetTest)
{
    UnsignedBits<3> bits;
    ASSERT_EQ(0u, bits.get());

    bits.set(1u);
    ASSERT_EQ(1u, bits.get());

    bits.set(4u);
    ASSERT_EQ(4u, bits.get());

    bits.set(7u);
    ASSERT_EQ(7u, bits.get());

    bits.set(8u);
    ASSERT_EQ(0u, bits.get());

    bits.set(15u);
    ASSERT_EQ(7u, bits.get());
}

TEST(PackTest, SizeTest)
{
    SignedBits<4> sbits4;
    SignedBits<3> sbits3;

    ASSERT_EQ(7, (sbits4, sbits3).size);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}