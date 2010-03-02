#ifndef BITS_H
#define BITS_H

namespace emattsan
{

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
    virtual void setSequence(unsigned_value_type value) = 0;
    virtual unsigned_value_type getSequence() const = 0;

    struct Pack
    {
        const Pack* prev;
        Bits&       bits;
        const int   size;

        explicit Pack(Bits& bits) : prev(0), bits(bits), size(bits.size()) {}

        Pack(const Pack& prev, Bits& bits) : prev(&prev), bits(bits), size(prev.size + bits.size()) {}

        Pack operator , (Bits& bits) { return Pack(*this, bits); }

        Pack& set(unsigned_value_type value)
        {
            for(const Pack* pack = this; pack; pack = pack->prev)
            {
                pack->bits.setSequence(value);
                value >>= pack->bits.size();
            }
            return *this;
        }

        unsigned_value_type getSequence() const
        {
            return prev ? (prev->getSequence() << bits.size() | bits.getSequence()) : bits.getSequence();
        }

        Pack& operator = (unsigned_value_type value) { return set(value); }

        operator unsigned_value_type () const { return getSequence(); }
    };

    Pack operator , (Bits& bits) { return Pack(Pack(*this), bits); }
};

template<int SIZE, typename T = unsigned int>
class SignedBits : public Bits<T>
{
public:
    typedef typename Bits<T>::signed_value_type   signed_value_type;
    typedef typename Bits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Bits<T>::signed_value_type   value_type;

    static const int Size = SIZE;

    explicit SignedBits(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    SignedBits& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const { return value_; }

    void setSequence(unsigned_value_type value) { set(value); }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(value_) & ((1u << Size) - 1);
    };

    SignedBits& operator = (value_type n) { return set(n); }

    operator value_type () const { return get(); }

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
    typedef typename Bits<T>::signed_value_type   signed_value_type;
    typedef typename Bits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Bits<T>::unsigned_value_type value_type;

    static const int Size = SIZE;

    explicit UnsignedBits(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    UnsignedBits& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const { return value_; }

    void setSequence(unsigned_value_type value) { set(value); }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(value_) & ((1u << Size) - 1);
    };

    UnsignedBits& operator = (value_type n) { return set(n); }

    operator value_type () const { return get(); }

private:
    value_type trim(value_type n)
    {
        return n & ((1u << Size) - 1);
    }

    value_type value_;
};

} // namespace emattsan

#endif//BITS_H
