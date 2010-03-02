#ifndef BITS_H
#define BITS_H

template<typename T> struct Traits;

template<> struct Traits<char>
{
    typedef signed char   signed_value_type;
    typedef unsigned char unsigned_value_type;
};

template<> struct Traits<signed char>
{
    typedef signed char   signed_value_type;
    typedef unsigned char unsigned_value_type;
};

template<> struct Traits<unsigned char>
{
    typedef signed char   signed_value_type;
    typedef unsigned char unsigned_value_type;
};

template<> struct Traits<signed short>
{
    typedef signed short   signed_value_type;
    typedef unsigned short unsigned_value_type;
};

template<> struct Traits<unsigned short>
{
    typedef signed short   signed_value_type;
    typedef unsigned short unsigned_value_type;
};

template<> struct Traits<signed int>
{
    typedef signed int   signed_value_type;
    typedef unsigned int unsigned_value_type;
};

template<> struct Traits<unsigned int>
{
    typedef signed int   signed_value_type;
    typedef unsigned int unsigned_value_type;
};

template<> struct Traits<signed long>
{
    typedef signed long   signed_value_type;
    typedef unsigned long unsigned_value_type;
};

template<> struct Traits<unsigned long>
{
    typedef signed long   signed_value_type;
    typedef unsigned long unsigned_value_type;
};

template<typename T>
class Bits
{
public:
    typedef typename Traits<T>::signed_value_type   signed_value_type;
    typedef typename Traits<T>::unsigned_value_type unsigned_value_type;

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

template<int SIZE, typename T = unsigned int>
class SignedBits : public Bits<T>
{
public:
    typedef typename Bits<T>::signed_value_type   value_type;

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

template<int SIZE, typename T = unsigned long>
class UnsignedBits : public Bits<T>
{
public:
    typedef typename Bits<T>::unsigned_value_type value_type;

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
