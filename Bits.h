#ifndef EMATTSAN_BITS_H
#define EMATTSAN_BITS_H

namespace emattsan
{

namespace bits
{

namespace detail
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

template<int N>
struct Width
{
    template<bool LE_CHAR_SIZE, bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE> struct _
    {
        typedef Traits<long>::unsigned_value_type value_type;
    };

    template<bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE> struct _<true, LE_SHORT_SIZE, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<char>::unsigned_value_type value_type;
    };

    template<bool LE_INT_SIZE, bool LE_LONG_SIZE> struct _<false, true, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<short>::unsigned_value_type value_type;
    };

    template<bool LE_LONG_SIZE> struct _<false, false, true, LE_LONG_SIZE>
    {
        typedef Traits<int>::unsigned_value_type value_type;
    };

    static const int BitsPerChar = 8;

    typedef typename _< (N <= BitsPerChar),
                        (N <= (sizeof(short) * BitsPerChar)),
                        (N <= (sizeof(int) * BitsPerChar)),
                        (N <= (sizeof(long) * BitsPerChar))
                      >::value_type value_type;
};

template<int N> struct Reserved;

} // namespace detail

template<class LHS, class RHS> class Pack;
template<class LHS, class RHS> class ConstPack;

template<int SIZE, typename T = unsigned int>
class Signed
{
public:
    typedef typename detail::Traits<T>::signed_value_type   signed_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type unsigned_value_type;
    typedef typename detail::Traits<T>::signed_value_type   value_type;

    static const int                 Size = SIZE;
    static const unsigned_value_type Mask = ((1u << Size) - 1);

    explicit Signed(value_type n = 0) : value_(trim(n))
    {
    }

    static int size()
    {
        return Size;
    }

    Signed& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const
    {
        return value_;
    }

    Signed& operator = (value_type n)
    {
        return set(n);
    }

    operator value_type () const
    {
        return get();
    }

    void setSequence(unsigned_value_type value)
    {
        value_ = trim(value);
    }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(value_) & Mask;
    };

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
class Unsigned
{
public:
    typedef typename detail::Traits<T>::signed_value_type   signed_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type unsigned_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type value_type;

    static const int                 Size = SIZE;
    static const unsigned_value_type Mask = ((1u << Size) - 1);

    explicit Unsigned(value_type n = 0) : value_(trim(n))
    {
    }

    static int size()
    {
        return Size;
    }

    Unsigned& set(value_type n)
    {
        value_ = trim(n);
        return *this;
    }

    value_type get() const
    {
        return value_;
    }

    Unsigned& operator = (value_type n)
    {
        return set(n);
    }

    operator value_type () const
    {
        return get();
    }

    void setSequence(unsigned_value_type value)
    {
        value_ = trim(value);
    }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(value_) & Mask;
    };

private:
    value_type trim(value_type n)
    {
        return n & ((1u << Size) - 1);
    }

    value_type value_;
};

template<class LHS, class RHS>
class Pack
{
public:
    static const int Size = LHS::Size + RHS::Size;

    typedef typename detail::Width<Size>::value_type value_type;

    Pack(LHS& lhs, RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
    }

    static int size()
    {
        return Size;
    }

    void setSequence(value_type value)
    {
        rhs_.setSequence(value);
        lhs_.setSequence(value >> RHS::Size);
    }

    value_type getSequence() const
    {
        return (lhs_.getSequence() << RHS::Size) | rhs_.getSequence();
    }

    Pack& operator = (value_type value)
    {
        setSequence(value);
        return *this;
    }

    operator value_type () const
    {
        return getSequence();
    }

    template<class R>
    Pack<Pack, R> operator , (R& rhs)
    {
        return Pack<Pack, R>(*this, rhs);
    }

    template<class R>
    ConstPack<Pack, R> operator , (const R& rhs) const
    {
        return ConstPack<Pack, R>(*this, rhs);
    }

private:
    LHS& lhs_;
    RHS& rhs_;
};

template<class LHS, int N>
class Pack<LHS, detail::Reserved<N>*(*)()>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    Pack(LHS& lhs, detail::Reserved<N>*(*)()) : lhs_(lhs)
    {
    }

    static int size()
    {
        return Size;
    }

    void setSequence(value_type value)
    {
        lhs_.setSequence(value >> N);
    }

    value_type getSequence() const
    {
        return lhs_.getSequence() << N;
    }

    Pack& operator = (value_type value)
    {
        setSequence(value); return *this;
    }

    operator value_type () const
    {
        return getSequence();
    }

    template<class R>
    Pack<Pack, R> operator , (R& rhs)
    {
        return Pack<Pack, R>(*this, rhs);
    }

    template<class R>
    ConstPack<Pack, R> operator , (const R& rhs) const
    {
        return ConstPack<Pack, R>(*this, rhs);
    }

private:
    LHS& lhs_;
};

template<class LHS, class RHS>
class ConstPack
{
public:
    static const int Size = LHS::Size + RHS::Size;

    typedef typename detail::Width<Size>::value_type value_type;

    ConstPack(const LHS& lhs, const RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
    }

    static int size()
    {
        return Size;
    }

    value_type getSequence() const
    {
        return (lhs_.getSequence() << RHS::Size) | rhs_.getSequence();
    }

    operator value_type () const
    {
        return getSequence();
    }

private:
    const LHS& lhs_;
    const RHS& rhs_;
};

template<class LHS, int N>
class ConstPack<LHS, detail::Reserved<N>*(*)()>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    ConstPack(const LHS& lhs, detail::Reserved<N>*(*)()) : lhs_(lhs)
    {
    }

    static int size()
    {
        return Size;
    }

    value_type getSequence() const
    {
        return lhs_.getSequence() << N;
    }

    operator value_type () const
    {
        return getSequence();
    }

private:
    const LHS& lhs_;
};

template<typename LHS, typename RHS>
Pack<LHS, RHS> operator , (LHS& lhs, RHS& rhs)
{
    return Pack<LHS, RHS>(lhs, rhs);
}

template<typename LHS, int N>
Pack<LHS, detail::Reserved<N>*(*)()> operator , (LHS& lhs, detail::Reserved<N>* (*rhs)())
{
    return Pack<LHS, detail::Reserved<N>*(*)()>(lhs, rhs);
}

template<typename LHS, typename RHS>
ConstPack<LHS, RHS> operator , (const LHS& lhs, const RHS& rhs)
{
    return ConstPack<LHS, RHS>(lhs, rhs);
}

template<typename LHS, int N>
ConstPack<LHS, detail::Reserved<N>*(*)()> operator , (const LHS& lhs, detail::Reserved<N>* (*rhs)())
{
    return ConstPack<LHS, detail::Reserved<N>*(*)()>(lhs, rhs);
}

template<int N>
detail::Reserved<N>* reserve()
{
    return 0;
}

} // namespace bits

} // namespace emattsan

#endif//EMATTSAN_BITS_H
