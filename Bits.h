#ifndef EMATTSAN_BITS_H
#define EMATTSAN_BITS_H

#include <limits>

namespace emattsan
{

namespace bits
{

template<int SIZE, typename T> class Signed;
template<int SIZE, typename T> class Unsigned;

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

    typedef typename _< (N <= std::numeric_limits<unsigned char>::digits),
                        (N <= std::numeric_limits<unsigned short>::digits),
                        (N <= std::numeric_limits<unsigned int>::digits),
                        (N <= std::numeric_limits<unsigned long>::digits)
                      >::value_type value_type;
};

template<typename T, int N>
struct Mask
{
    template<int M, bool F> struct _   { static const T value = ((1u << M) - 1);    };
    template<int M> struct _<M, false> { static const T value = static_cast<T>(-1); };

    static const T value = _<N, N < std::numeric_limits<T>::digits>::value;
};

template<int L, int R>
struct Max
{
    template <int L_, int R_, bool F> struct _       { static const int value = L_; };
    template <int L_, int R_> struct _<L_, R_, true> { static const int value = R_; };

    static const int value = _<L, R, (L < R)>::value;
};

template<int N, int M>
struct Greater
{
    typedef Signed<detail::Max<N, M>::value, typename detail::Width<detail::Max<N, M>::value>::value_type>   signed_type;
    typedef Unsigned<detail::Max<N, M>::value, typename detail::Width<detail::Max<N, M>::value>::value_type> unsigned_type;
};

template<int SIZE, typename T, typename BITS, typename VALUE_TYPE>
class Base
{
public:
    typedef BITS Bits;

    typedef typename detail::Traits<T>::signed_value_type   signed_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type unsigned_value_type;
    typedef VALUE_TYPE                      value_type;

    static const int                 Size = SIZE;
    static const unsigned_value_type Mask = detail::Mask<unsigned_value_type, Size>::value;

    static int size()
    {
        return Size;
    }

    Bits& set(value_type n)
    {
        self().value_ = self().trim(n);
        return self();
    }

    value_type get() const
    {
        return self().value_;
    }

    operator value_type () const
    {
        return get();
    }

    void setSequence(unsigned_value_type value)
    {
        self().value_ = self().trim(value);
    }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(self().value_) & Mask;
    };

protected:
    Base()
    {
    }

private:
    Bits& self()
    {
        return static_cast<Bits&>(*this);
    }

    const Bits& self() const
    {
        return static_cast<const Bits&>(*this);
    }
};

template<int N> struct Reserved;

} // namespace detail

template<int SIZE, typename T = unsigned int>
class Signed : public detail::Base<SIZE, T, Signed<SIZE, T>, typename detail::Traits<T>::signed_value_type>
{
public:
    typedef detail::Base<SIZE, T, Signed<SIZE, T>, typename detail::Traits<T>::signed_value_type> super;

    typedef typename super::signed_value_type   signed_value_type;
    typedef typename super::unsigned_value_type unsigned_value_type;
    typedef typename super::value_type          value_type;

    Signed() : super(), value_(0)
    {
    }

    explicit Signed(value_type n) : super(), value_(trim(n))
    {
    }

    template<int M, typename U>
    Signed(const Signed<M, U>& bits) : super(), value_(trim(bits.get()))
    {
    }

    template<int M, typename U>
    Signed(const Unsigned<M, U>& bits) : super(), value_(trim(bits.get()))
    {
    }

    Signed& operator = (value_type n)
    {
        return set(n);
    }

    Signed& operator += (value_type n)
    {
        return set(value_ + n);
    }

    Signed& operator -= (value_type n)
    {
        return set(value_ - n);
    }

    Signed& operator *= (value_type n)
    {
        return set(value_ * n);
    }

    Signed& operator /= (value_type n)
    {
        return set(value_ / n);
    }

private:
    static value_type trim(value_type n)
    {
        n <<= (std::numeric_limits<unsigned_value_type>::digits - super::Size);
        n >>= (std::numeric_limits<unsigned_value_type>::digits - super::Size);
        return n;
    }

    value_type value_;

    friend class detail::Base<SIZE, T, Signed<SIZE, T>, typename detail::Traits<T>::signed_value_type>;
};

template<int SIZE, typename T = unsigned int>
class Unsigned : public detail::Base<SIZE, T, Unsigned<SIZE, T>, typename detail::Traits<T>::unsigned_value_type>
{
public:
    typedef detail::Base<SIZE, T, Unsigned<SIZE, T>, typename detail::Traits<T>::unsigned_value_type> super;

    typedef typename super::signed_value_type   signed_value_type;
    typedef typename super::unsigned_value_type unsigned_value_type;
    typedef typename super::value_type          value_type;

    Unsigned() : super(), value_(0)
    {
    }

    explicit Unsigned(value_type n) : super(), value_(trim(n))
    {
    }

    template<int M, typename U>
    Unsigned(const Signed<M, U>& bits) : super(), value_(trim(bits.get()))
    {
    }

    template<int M, typename U>
    Unsigned(const Unsigned<M, U>& bits) : super(), value_(trim(bits.get()))
    {
    }

    Unsigned& operator = (value_type n)
    {
        return set(n);
    }

    Unsigned& operator += (value_type n)
    {
        return set(value_ + n);
    }

    Unsigned& operator -= (value_type n)
    {
        return set(value_ - n);
    }

    Unsigned& operator *= (value_type n)
    {
        return set(value_ * n);
    }

    Unsigned& operator /= (value_type n)
    {
        return set(value_ / n);
    }

private:
    static value_type trim(value_type n)
    {
        n &= super::Mask;
        return n;
    }

    value_type value_;

    friend class detail::Base<SIZE, T, Unsigned<SIZE, T>, typename detail::Traits<T>::unsigned_value_type>;
};

template<typename LHS, typename RHS> class ConstPack;

template<typename LHS, typename RHS>
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

    template<int M, typename U>
    Pack<Pack, Signed<M, U> > operator , (Signed<M, U>& rhs)
    {
        return Pack<Pack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    Pack<Pack, Unsigned<M, U> > operator , (Unsigned<M, U>& rhs)
    {
        return Pack<Pack, Unsigned<M, U> >(*this, rhs);
    }

    template<int N>
    Pack<Pack, void (*)(detail::Reserved<N>*)> operator , (void (*rhs)(detail::Reserved<N>*))
    {
        return Pack<Pack, void (*)(detail::Reserved<N>*)>(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<Pack, Signed<M, U> > operator , (const Signed<M, U>& rhs) const
    {
        return ConstPack<Pack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<Pack, Unsigned<M, U> > operator , (const Unsigned<M, U>& rhs) const
    {
        return ConstPack<Pack, Unsigned<M, U> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<Pack, Pack<L, R> > operator , (const Pack<L, R>& rhs) const
    {
        return ConstPack<Pack, Pack<L, R> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<Pack, ConstPack<L, R> > operator , (const ConstPack<L, R>& rhs) const
    {
        return ConstPack<Pack, ConstPack<L, R> >(*this, rhs);
    }

    template<int N>
    ConstPack<Pack, void (*)(detail::Reserved<N>*)> operator , (void (*rhs)(detail::Reserved<N>*)) const
    {
        return ConstPack<Pack, void (*)(detail::Reserved<N>*)>(*this, rhs);
    }

private:
    LHS& lhs_;
    RHS& rhs_;
};

template<typename LHS, int N>
class Pack<LHS, void (*)(detail::Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    Pack(LHS& lhs, void (*)(detail::Reserved<N>*)) : lhs_(lhs)
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
        setSequence(value);
        return *this;
    }

    operator value_type () const
    {
        return getSequence();
    }

    template<int M, typename U>
    Pack<Pack, Signed<M, U> > operator , (Signed<M, U>& rhs)
    {
        return Pack<Pack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    Pack<Pack, Unsigned<M, U> > operator , (Unsigned<M, U>& rhs)
    {
        return Pack<Pack, Unsigned<M, U> >(*this, rhs);
    }

    template<int M>
    Pack<Pack, void (*)(detail::Reserved<M>*)> operator , (void (*rhs)(detail::Reserved<M>*))
    {
        return Pack<Pack, void (*)(detail::Reserved<M>*)>(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<Pack, Signed<M, U> > operator , (const Signed<M, U>& rhs) const
    {
        return ConstPack<Pack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<Pack, Unsigned<M, U> > operator , (const Unsigned<M, U>& rhs) const
    {
        return ConstPack<Pack, Unsigned<M, U> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<Pack, Pack<L, R> > operator , (const Pack<L, R>& rhs) const
    {
        return ConstPack<Pack, Pack<L, R> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<Pack, ConstPack<L, R> > operator , (const ConstPack<L, R>& rhs) const
    {
        return ConstPack<Pack, ConstPack<L, R> >(*this, rhs);
    }

    template<int M>
    ConstPack<Pack, void (*)(detail::Reserved<M>*)> operator , (void (*rhs)(detail::Reserved<M>*)) const
    {
        return ConstPack<Pack, void (*)(detail::Reserved<M>*)>(*this, rhs);
    }

private:
    LHS& lhs_;
};

template<typename LHS, typename RHS>
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

    template<int M, typename U>
    ConstPack<ConstPack, Signed<M, U> > operator , (const Signed<M, U>& rhs) const
    {
        return ConstPack<ConstPack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<ConstPack, Unsigned<M, U> > operator , (const Unsigned<M, U>& rhs) const
    {
        return ConstPack<ConstPack, Unsigned<M, U> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<ConstPack, Pack<L, R> > operator , (const Pack<L, R>& rhs) const
    {
        return ConstPack<ConstPack, Pack<L, R> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<ConstPack, ConstPack<L, R> > operator , (const ConstPack<L, R>& rhs) const
    {
        return ConstPack<ConstPack, ConstPack<L, R> >(*this, rhs);
    }

    template<int N>
    ConstPack<ConstPack, void (*)(detail::Reserved<N>*)> operator , (void (*rhs)(detail::Reserved<N>*)) const
    {
        return ConstPack<ConstPack, void (*)(detail::Reserved<N>*)>(*this, rhs);
    }

private:
    const LHS& lhs_;
    const RHS& rhs_;
};

template<typename LHS, int N>
class ConstPack<LHS, void (*)(detail::Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    ConstPack(const LHS& lhs, void (*)(detail::Reserved<N>*)) : lhs_(lhs)
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

    template<int M, typename U>
    ConstPack<ConstPack, Signed<M, U> > operator , (const Signed<M, U>& rhs) const
    {
        return ConstPack<ConstPack, Signed<M, U> >(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<ConstPack, Unsigned<M, U> > operator , (const Unsigned<M, U>& rhs) const
    {
        return ConstPack<ConstPack, Unsigned<M, U> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<ConstPack, Pack<L, R> > operator , (const Pack<L, R>& rhs) const
    {
        return ConstPack<ConstPack, Pack<L, R> >(*this, rhs);
    }

    template<typename L, typename R>
    ConstPack<ConstPack, ConstPack<L, R> > operator , (const ConstPack<L, R>& rhs) const
    {
        return ConstPack<ConstPack, ConstPack<L, R> >(*this, rhs);
    }

    template<int M>
    ConstPack<ConstPack, void (*)(detail::Reserved<M>*)> operator , (void (*rhs)(detail::Reserved<M>*)) const
    {
        return ConstPack<ConstPack, void (*)(detail::Reserved<M>*)>(*this, rhs);
    }

private:
    const LHS& lhs_;
};

#define DEFINE_OP(op) \
template<int N, typename T, int M, typename U> inline typename detail::Greater<N, M>::signed_type operator op (const Signed<N, T>& lhs, const Signed<M, U>& rhs) \
{ return typename detail::Greater<N, M>::signed_type(lhs.get() op rhs.get()); } \
template<int N, typename T, int M, typename U> inline typename detail::Greater<N, M>::unsigned_type operator op (const Signed<N, T>& lhs, const Unsigned<M, U>& rhs) \
{ return typename detail::Greater<N, M>::unsigned_type(lhs.get() op rhs.get()); } \
template<int N, typename T, int M, typename U> inline typename detail::Greater<N, M>::unsigned_type operator op (const Unsigned<N, T>& lhs, const Signed<M, U>& rhs) \
{ return typename detail::Greater<N, M>::unsigned_type(lhs.get() op rhs.get()); } \
template<int N, typename T, int M, typename U> inline typename detail::Greater<N, M>::unsigned_type operator op (const Unsigned<N, T>& lhs, const Unsigned<M, U>& rhs) \
{ return typename detail::Greater<N, M>::unsigned_type(lhs.get() op rhs.get()); } \
template<int N, typename T> inline Signed<N, T> operator op (const Signed<N, T>& lhs, typename Signed<N, T>::signed_value_type rhs) \
{ return lhs op Signed<N, T>(rhs); } \
template<int N, typename T> inline Signed<N, T> operator op (typename Signed<N, T>::signed_value_type lhs, const Signed<N, T>& rhs) \
{ return Signed<N, T>(lhs) op rhs; } \
template<int N, typename T> inline Unsigned<N, T> operator op (const Signed<N, T>& lhs, typename Signed<N, T>::unsigned_value_type rhs) \
{ return lhs op Unsigned<N, T>(rhs); } \
template<int N, typename T> inline Unsigned<N, T> operator op (typename Signed<N, T>::unsigned_value_type lhs, const Signed<N, T>& rhs) \
{ return Unsigned<N, T>(lhs) op rhs; } \
template<int N, typename T> inline Unsigned<N, T> operator op (const Unsigned<N, T>& lhs, typename Unsigned<N, T>::signed_value_type rhs) \
{ return lhs op Signed<N, T>(rhs); } \
template<int N, typename T> inline Unsigned<N, T> operator op (typename Unsigned<N, T>::signed_value_type lhs, const Unsigned<N, T>& rhs) \
{ return Signed<N, T>(lhs) op rhs; } \
template<int N, typename T> inline Unsigned<N, T> operator op (const Unsigned<N, T>& lhs, typename Unsigned<N, T>::unsigned_value_type rhs) \
{ return lhs op Unsigned<N, T>(rhs); } \
template<int N, typename T> inline Unsigned<N, T> operator op (typename Unsigned<N, T>::unsigned_value_type lhs, const Unsigned<N, T>& rhs) \
{ return Unsigned<N, T>(lhs) op rhs; }

DEFINE_OP(+)
DEFINE_OP(-)
DEFINE_OP(*)
DEFINE_OP(/)

#undef DEFINE_OP

template<int N, typename T, int M, typename U>
Pack<Signed<N, T>, Signed<M, U> > operator , (Signed<N, T>& lhs, Signed<M, U>& rhs)
{
    return Pack<Signed<N, T>, Signed<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
Pack<Signed<N, T>, Unsigned<M, U> > operator , (Signed<N, T>& lhs, Unsigned<M, U>& rhs)
{
    return Pack<Signed<N, T>, Unsigned<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
Pack<Unsigned<N, T>, Signed<M, U> > operator , (Unsigned<N, T>& lhs, Signed<M, U>& rhs)
{
    return Pack<Unsigned<N, T>, Signed<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
Pack<Unsigned<N, T>, Unsigned<M, U> > operator , (Unsigned<N, T>& lhs, Unsigned<M, U>& rhs)
{
    return Pack<Unsigned<N, T>, Unsigned<M, U> >(lhs, rhs);
}

template<int N, typename T, int M>
Pack<Signed<N, T>, void (*)(detail::Reserved<M>*)> operator , (Signed<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return Pack<Signed<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N, typename T, int M>
Pack<Unsigned<N, T>, void (*)(detail::Reserved<M>*)> operator , (Unsigned<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return Pack<Unsigned<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N, typename T, int M, typename U>
ConstPack<Signed<N, T>, Signed<M, U> > operator , (const Signed<N, T>& lhs, const Signed<M, U>& rhs)
{
    return ConstPack<Signed<N, T>, Signed<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
ConstPack<Signed<N, T>, Unsigned<M, U> > operator , (const Signed<N, T>& lhs, const Unsigned<M, U>& rhs)
{
    return ConstPack<Signed<N, T>, Unsigned<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
ConstPack<Unsigned<N, T>, Signed<M, U> > operator , (const Unsigned<N, T>& lhs, const Signed<M, U>& rhs)
{
    return ConstPack<Unsigned<N, T>, Signed<M, U> >(lhs, rhs);
}

template<int N, typename T, int M, typename U>
ConstPack<Unsigned<N, T>, Unsigned<M, U> > operator , (const Unsigned<N, T>& lhs, const Unsigned<M, U>& rhs)
{
    return ConstPack<Unsigned<N, T>, Unsigned<M, U> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Signed<N, T>, Pack<L, R> > operator , (const Signed<N, T>& lhs, const Pack<L, R>& rhs)
{
    return ConstPack<Signed<N, T>, Pack<L, R> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Unsigned<N, T>, Pack<L, R> > operator , (const Unsigned<N, T>& lhs, const Pack<L, R>& rhs)
{
    return ConstPack<Unsigned<N, T>, Pack<L, R> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Signed<N, T>, ConstPack<L, R> > operator , (const Signed<N, T>& lhs, const ConstPack<L, R>& rhs)
{
    return ConstPack<Signed<N, T>, ConstPack<L, R> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Unsigned<N, T>, ConstPack<L, R> > operator , (const Unsigned<N, T>& lhs, const ConstPack<L, R>& rhs)
{
    return ConstPack<Unsigned<N, T>, ConstPack<L, R> >(lhs, rhs);
}

template<int N, typename T, int M>
ConstPack<Signed<N, T>, void (*)(detail::Reserved<M>*)> operator , (const Signed<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return ConstPack<Signed<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N, typename T, int M>
ConstPack<Unsigned<N, T>, void (*)(detail::Reserved<M>*)> operator , (const Unsigned<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return ConstPack<Unsigned<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N> void reserve(detail::Reserved<N>*) {}

} // namespace bits

} // namespace emattsan

#endif//EMATTSAN_BITS_H
