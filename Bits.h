#ifndef BITS_H
#define BITS_H

class Bits
{
public:
    virtual ~Bits() {}
    virtual int size() const = 0;

    struct Pack
    {
        const Pack* const prev;
        const Bits&       bits;
        const int         size;

        explicit Pack(const Bits& bits) : prev(0), bits(bits), size(bits.size())
        {
        }

        Pack(const Pack& prev, const Bits& bits) : prev(&prev),
                                                   bits(bits),
                                                   size(prev.size + bits.size())
        {
        }

        Pack operator , (const Bits& bits) const
        {
            return Pack(*this, bits);
        }
    };

    Pack operator , (const Bits& bits) const
    {
        return Pack(Pack(*this), bits);
    }
};

template<int SIZE>
class SignedBits : public Bits
{
public:
    typedef int value_type;

    static const int Size = SIZE;

    explicit SignedBits(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    SignedBits& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const
    {
        return value_;
    }

private:
    value_type trim(value_type n)
    {
        n <<= ((sizeof(n) * 8) - Size);
        n >>= ((sizeof(n) * 8) - Size);
        return n;
    }

    value_type value_;
};

template<int SIZE>
class UnsignedBits : public Bits
{
public:
    typedef unsigned int value_type;

    static const int Size = SIZE;

    explicit UnsignedBits(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    UnsignedBits& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const
    {
        return value_;
    } 

private:
    value_type trim(value_type n)
    {
        return n & ((1u << Size) - 1);
    }

    value_type value_;
};

#endif//BITS_H
