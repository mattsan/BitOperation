#ifndef EMATTSAN_BITS_H
#define EMATTSAN_BITS_H

#include <limits>

namespace emattsan
{

namespace bits
{

template<int SIZE, typename T> class Bits;

namespace detail
{

struct Signed;
struct Unsigned;

template<typename T> struct Traits;

template<> struct Traits<signed char>
{
    typedef signed char   signed_value_type;
    typedef unsigned char unsigned_value_type;
    typedef signed char   value_type;
    typedef Signed        sign_type;
};

template<> struct Traits<unsigned char>
{
    typedef signed char   signed_value_type;
    typedef unsigned char unsigned_value_type;
    typedef unsigned char value_type;
    typedef Unsigned      sign_type;
};

template<> struct Traits<signed short>
{
    typedef signed short   signed_value_type;
    typedef unsigned short unsigned_value_type;
    typedef signed short   value_type;
    typedef Signed        sign_type;
};

template<> struct Traits<unsigned short>
{
    typedef signed short   signed_value_type;
    typedef unsigned short unsigned_value_type;
    typedef unsigned short value_type;
    typedef Unsigned       sign_type;
};

template<> struct Traits<signed int>
{
    typedef signed int   signed_value_type;
    typedef unsigned int unsigned_value_type;
    typedef signed int   value_type;
    typedef Signed        sign_type;
};

template<> struct Traits<unsigned int>
{
    typedef signed int   signed_value_type;
    typedef unsigned int unsigned_value_type;
    typedef unsigned int value_type;
    typedef Unsigned     sign_type;
};

template<> struct Traits<signed long>
{
    typedef signed long   signed_value_type;
    typedef unsigned long unsigned_value_type;
    typedef signed long   value_type;
    typedef Signed        sign_type;
};

template<> struct Traits<unsigned long>
{
    typedef signed long   signed_value_type;
    typedef unsigned long unsigned_value_type;
    typedef unsigned long value_type;
    typedef Unsigned      sign_type;
};

template<int N>
struct Width
{
    template<bool LE_CHAR_SIZE, bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _
    {
        typedef Traits<unsigned long>::signed_value_type   signed_value_type;
        typedef Traits<unsigned long>::unsigned_value_type unsigned_value_type;
        typedef Traits<unsigned long>::unsigned_value_type value_type;
    };

    template<bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<true, LE_SHORT_SIZE, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<unsigned char>::signed_value_type   signed_value_type;
        typedef Traits<unsigned char>::unsigned_value_type unsigned_value_type;
        typedef Traits<unsigned char>::unsigned_value_type value_type;
    };

    template<bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<false, true, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<unsigned short>::signed_value_type   signed_value_type;
        typedef Traits<unsigned short>::unsigned_value_type unsigned_value_type;
        typedef Traits<unsigned short>::unsigned_value_type value_type;
    };

    template<bool LE_LONG_SIZE>
    struct _<false, false, true, LE_LONG_SIZE>
    {
        typedef Traits<unsigned int>::signed_value_type   signed_value_type;
        typedef Traits<unsigned int>::unsigned_value_type unsigned_value_type;
        typedef Traits<unsigned int>::unsigned_value_type value_type;
    };

     typedef _< (N <= std::numeric_limits<unsigned char>::digits),
                (N <= std::numeric_limits<unsigned short>::digits),
                (N <= std::numeric_limits<unsigned int>::digits),
                (N <= std::numeric_limits<unsigned long>::digits)
              > type;

    typedef typename type::signed_value_type   signed_value_type;
    typedef typename type::unsigned_value_type unsigned_value_type;
    typedef typename type::value_type          value_type;
};

template<typename T, int N>
struct Mask
{
    template<int M, bool F>
    struct _
    {
        static const T value = ((1u << M) - 1);
    };

    template<int M>
    struct _<M, false>
    {
        static const T value = static_cast<T>(-1);
    };

    static const T value = _<N, N < std::numeric_limits<T>::digits>::value;
};

template<int L, int R>
struct Max
{
    template <int L_, int R_, bool F> struct _       { static const int value = L_; };
    template <int L_, int R_> struct _<L_, R_, true> { static const int value = R_; };

    static const int value = _<L, R, (L < R)>::value;
};

template<int SIZE, typename T, typename SIGN> class BitsBase;

template<int SIZE, typename T>
class BitsBase<SIZE, T, Signed>
{
public:
    typedef typename Traits<T>::signed_value_type   signed_value_type;
    typedef typename Traits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Traits<T>::value_type          value_type;

    static const int                 Size = SIZE;
    static const unsigned_value_type Mask = Mask<unsigned_value_type, Size>::value;

    BitsBase(value_type value) : value_(trim(value))
    {
    }

    static value_type trim(value_type n)
    {
        n <<= (std::numeric_limits<unsigned_value_type>::digits - Size);
        n >>= (std::numeric_limits<unsigned_value_type>::digits - Size);
        return n;
    }

    value_type value_;
};

template<int SIZE, typename T>
class BitsBase<SIZE, T, Unsigned>
{
public:
    typedef typename Traits<T>::signed_value_type   signed_value_type;
    typedef typename Traits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Traits<T>::value_type          value_type;

    static const int                 Size = SIZE;
    static const unsigned_value_type Mask = Mask<unsigned_value_type, Size>::value;

    BitsBase(value_type value) : value_(trim(value))
    {
    }

    static value_type trim(value_type n)
    {
        n &= Mask;
        return n;
    }

    value_type value_;
};

template<int N, int M, typename T, typename U>
struct Result
{
    template<int N1, int N2, typename T1, typename T2>
    struct _
    {
        static const int Size = Max<N1, N2>::value;
        typedef Bits<Size, typename Width<Size>::unsigned_value_type> result_type;
    };

    template<int N1, int N2>
    struct _<N1, N2, Signed, Signed>
    {
        static const int Size = Max<N1, N2>::value;
        typedef Bits<Size, typename Width<Size>::signed_value_type> result_type;
    };
    
    typedef typename _<N, M, typename Traits<T>::sign_type, typename Traits<U>::sign_type>::result_type result_type;
};

template<int N> struct Reserved;

template<typename LHS, typename RHS>
class PackBase
{
public:
    static const int Size = LHS::Size + RHS::Size;

    typedef typename detail::Width<Size>::value_type value_type;

    PackBase(LHS& lhs, RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
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

private:
    LHS& lhs_;
    RHS& rhs_;
};

template<typename LHS, int N>
class PackBase<LHS, void (*)(detail::Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    PackBase(LHS& lhs, void (*)(detail::Reserved<N>*)) : lhs_(lhs)
    {
    }

    void setSequence(value_type value)
    {
        lhs_.setSequence(value >> N);
    }

    value_type getSequence() const
    {
        return lhs_.getSequence() << N;
    }

private:
    LHS& lhs_;
};

template<typename LHS, typename RHS>
class ConstPackBase
{
public:
    static const int Size = LHS::Size + RHS::Size;

    typedef typename detail::Width<Size>::value_type value_type;

    ConstPackBase(const LHS& lhs, const RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
    }

    value_type getSequence() const
    {
        return (lhs_.getSequence() << RHS::Size) | rhs_.getSequence();
    }

private:
    const LHS& lhs_;
    const RHS& rhs_;
};

template<typename LHS, int N>
class ConstPackBase<LHS, void (*)(detail::Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef typename detail::Width<Size>::value_type value_type;

    ConstPackBase(const LHS& lhs, void (*)(detail::Reserved<N>*)) : lhs_(lhs)
    {
    }

    value_type getSequence() const
    {
        return lhs_.getSequence() << N;
    }

private:
    const LHS& lhs_;
};

} // namespace detail

template<int SIZE, typename T = unsigned int>
class Bits : private detail::BitsBase<SIZE, T, typename detail::Traits<T>::sign_type>
{
public:
    typedef detail::BitsBase<SIZE, T, typename detail::Traits<T>::sign_type> super;

    typedef typename super::signed_value_type   signed_value_type;
    typedef typename super::unsigned_value_type unsigned_value_type;
    typedef typename super::value_type          value_type;

    static const int                Size  = super::Size;
    static const unsigned_value_type Mask = super::Mask;

    static int size()
    {
        return Size;
    }

    Bits() : super(0)
    {
    }

    explicit Bits(value_type n) : super(n)
    {
    }

    template<int M, typename U>
    explicit Bits(const Bits<M, U>& bits) : super(bits.get())
    {
    }

    Bits& set(value_type n)
    {
        super::value_ = trim(n);
        return *this;
    }

    value_type get() const
    {
        return super::value_;
    }

    void setSequence(unsigned_value_type value)
    {
        super::value_ = trim(value);
    }

    unsigned_value_type getSequence() const
    {
        return static_cast<unsigned_value_type>(super::value_) & Mask;
    };

    operator value_type () const
    {
        return get();
    }

    Bits& operator = (value_type n)
    {
        return set(n);
    }

    Bits& operator += (value_type n)
    {
        return set(super::value_ + n);
    }

    Bits& operator -= (value_type n)
    {
        return set(super::value_ - n);
    }

    Bits& operator *= (value_type n)
    {
        return set(super::value_ * n);
    }

    Bits& operator /= (value_type n)
    {
        return set(super::value_ / n);
    }

    Bits& operator |= (value_type n)
    {
        return set(super::value_ | n);
    }

    Bits& operator &= (value_type n)
    {
        return set(super::value_ & n);
    }

    Bits& operator <<= (int n)
    {
        super::value_ = trim(super::value_ << n);
        return *this;
    }

    Bits& operator >>= (int n)
    {
        super::value_ = trim(super::value_ >> n);
        return *this;
    }

    friend inline Bits operator << (const Bits& lhs, int rhs)
    {
        return Bits(lhs.get() << rhs);
    }

    friend inline Bits operator >> (const Bits& lhs, int rhs)
    {
        return Bits(lhs.get() >> rhs);
    }
};

template<typename LHS, typename RHS> class ConstPack;

template<typename LHS, typename RHS>
class Pack : public detail::PackBase<LHS, RHS>
{
public:
    typedef detail::PackBase<LHS, RHS> super;

    static const int Size = super::Size;

    typedef typename super::value_type value_type;

    static int size()
    {
        return Size;
    }

    Pack(LHS& lhs, RHS& rhs) : super(lhs, rhs)
    {
    }

    Pack& operator = (value_type value)
    {
        setSequence(value);
        return *this;
    }

    Pack& operator = (const Pack& value)
    {
        setSequence(value.getSequence());
        return *this;
    }

    operator value_type () const
    {
        return super::getSequence();
    }

    template<int M, typename U>
    Pack<Pack, Bits<M, U> > operator , (Bits<M, U>& rhs)
    {
        return Pack<Pack, Bits<M, U> >(*this, rhs);
    }

    template<int M>
    Pack<Pack, void (*)(detail::Reserved<M>*)> operator , (void (*rhs)(detail::Reserved<M>*))
    {
        return Pack<Pack, void (*)(detail::Reserved<M>*)>(*this, rhs);
    }

    template<int M, typename U>
    ConstPack<Pack, Bits<M, U> > operator , (const Bits<M, U>& rhs) const
    {
        return ConstPack<Pack, Bits<M, U> >(*this, rhs);
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
};

template<typename LHS, typename RHS>
class ConstPack : public detail::ConstPackBase<LHS, RHS>
{
public:
    typedef detail::ConstPackBase<LHS, RHS> super;

    static const int Size = super::Size;

    typedef typename super::value_type value_type;

    static int size()
    {
        return Size;
    }

    ConstPack(const LHS& lhs, const RHS& rhs) : super(lhs, rhs)
    {
    }

    operator value_type () const
    {
        return super::getSequence();
    }

    template<int M, typename U>
    ConstPack<ConstPack, Bits<M, U> > operator , (const Bits<M, U>& rhs) const
    {
        return ConstPack<ConstPack, Bits<M, U> >(*this, rhs);
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
};

#define DEFINE_OP(op) \
template<int N, typename T, int M, typename U> \
inline typename detail::Result<N, M, T, U>::result_type operator op (const Bits<N, T>& lhs, const Bits<M, U>& rhs) \
{ return typename detail::Result<N, M, T, U>::result_type(lhs.get() op rhs.get()); } \
template<int N, typename T> \
inline typename detail::Result<N, N, T, typename Bits<N, T>::signed_value_type>::result_type operator op (const Bits<N, T>& lhs, typename Bits<N, T>::signed_value_type rhs) \
{ return lhs op Bits<N, typename Bits<N, T>::signed_value_type>(rhs); } \
template<int N, typename T> \
inline typename detail::Result<N, N, T, typename Bits<N, T>::signed_value_type>::result_type operator op (typename Bits<N, T>::signed_value_type lhs, const Bits<N, T>& rhs) \
{ return Bits<N, typename Bits<N, T>::signed_value_type>(lhs) op rhs; } \
template<int N, typename T> \
inline typename detail::Result<N, N, T, typename Bits<N, T>::unsigned_value_type>::result_type operator op (const Bits<N, T>& lhs, typename Bits<N, T>::unsigned_value_type rhs) \
{ return lhs op Bits<N, typename Bits<N, T>::unsigned_value_type>(rhs); } \
template<int N, typename T> \
inline typename detail::Result<N, N, T, typename Bits<N, T>::unsigned_value_type>::result_type operator op (typename Bits<N, T>::unsigned_value_type lhs, const Bits<N, T>& rhs) \
{ return Bits<N, typename Bits<N, T>::unsigned_value_type>(lhs) op rhs; }

DEFINE_OP(+)
DEFINE_OP(-)
DEFINE_OP(*)
DEFINE_OP(/)
DEFINE_OP(|)
DEFINE_OP(&)

#undef DEFINE_OP

template<int N, typename T, int M, typename U>
Pack<Bits<N, T>, Bits<M, U> > operator , (Bits<N, T>& lhs, Bits<M, U>& rhs)
{
    return Pack<Bits<N, T>, Bits<M, U> >(lhs, rhs);
}

template<int N, typename T, int M>
Pack<Bits<N, T>, void (*)(detail::Reserved<M>*)> operator , (Bits<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return Pack<Bits<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N, typename T, int M, typename U>
ConstPack<Bits<N, T>, Bits<M, U> > operator , (const Bits<N, T>& lhs, const Bits<M, U>& rhs)
{
    return ConstPack<Bits<N, T>, Bits<M, U> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Bits<N, T>, Pack<L, R> > operator , (const Bits<N, T>& lhs, const Pack<L, R>& rhs)
{
    return ConstPack<Bits<N, T>, Pack<L, R> >(lhs, rhs);
}

template<int N, typename T, typename L, typename R>
ConstPack<Bits<N, T>, ConstPack<L, R> > operator , (const Bits<N, T>& lhs, const ConstPack<L, R>& rhs)
{
    return ConstPack<Bits<N, T>, ConstPack<L, R> >(lhs, rhs);
}

template<int N, typename T, int M>
ConstPack<Bits<N, T>, void (*)(detail::Reserved<M>*)> operator , (const Bits<N, T>& lhs, void (*rhs)(detail::Reserved<M>*))
{
    return ConstPack<Bits<N, T>, void (*)(detail::Reserved<M>*)>(lhs, rhs);
}

template<int N> void reserve(detail::Reserved<N>*) {}

} // namespace bits

} // namespace emattsan

#endif//EMATTSAN_BITS_H
