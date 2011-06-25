all: BitsTest
	./BitsTest

BitsTest: BitsTest.cpp Bits.h
	g++ -I. -o BitsTest BitsTest.cpp gtest/gtest-all.cc
