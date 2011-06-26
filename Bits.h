#ifndef EMATTSAN_BITS_H
#define EMATTSAN_BITS_H

//----------------------------------------------------------------------

#include <limits>
#include <cstring>

//----------------------------------------------------------------------

namespace emattsan
{

//----------------------------------------------------------------------

namespace bits
{

//----------------------------------------------------------------------

struct Signed;   // only declaration; used template parameter for expressing number signed
struct Unsigned; // only declaration; used template parameter for expressing number unsigned

template<int SIZE, typename T> class Bits;

//----------------------------------------------------------------------

namespace detail
{

//----------------------------------------------------------------------

template<int N> struct Reserved; // only declaration; used expressing reserved bits size

//----------------------------------------------------------------------

template<int SIZE>
struct MultiByte
{
    typedef unsigned char block_type;

    typedef MultiByte        signed_value_type;
    typedef MultiByte        unsigned_value_type;
    typedef MultiByte        value_type;
    typedef Unsigned         sign_type;
    typedef const MultiByte& arg_type;
    typedef const MultiByte& const_arg_type;
    typedef MultiByte&       ref_arg_type;
    typedef MultiByte&       result_type;
    typedef const MultiByte& const_result_type;
    typedef block_type       mask_type;

    static const int          BlockSize = std::numeric_limits<block_type>::digits;
    static const int          Size      = SIZE;
    static const int          Length    = (Size + BlockSize - 1) / BlockSize;
    static const unsigned int Capacity  = Length * BlockSize;

    MultiByte()
    {
        std::memset(value_, 0, Length);
    }

    MultiByte(const MultiByte& other)
    {
        std::memcpy(value_, other.value_, Length);
    }

    template<int M>
    MultiByte(const MultiByte<M>& other)
    {
        if(Length < M)
        {
            std::memcpy(value_, (other.value_ + M - Length), Length);
        }
        else
        {
            std::memset(value_, 0, Length - M);
            std::memcpy((value_ + Length - M), other.value_, M);
        }
    }

    int bitAt(int pos) const
    {
        return ((0 <= pos) && (pos < Size)) ? (value_[Length - (pos / BlockSize) - 1] >> (pos % BlockSize)) : 0;
    }

    block_type blockAt(int pos) const
    {
        return ((0 <= pos) && (pos < Length)) ? value_[pos] : 0;
    }

    block_type value_[Length];
};

//----------------------------------------------------------------------

template<typename T>
struct TraitsPrimitive;

template<>
struct TraitsPrimitive<signed char>
{
    typedef signed   char  signed_value_type;
    typedef unsigned char  unsigned_value_type;
    typedef signed   char  value_type;
    typedef Signed         sign_type;
};

template<>
struct TraitsPrimitive<unsigned char>
{
    typedef signed   char  signed_value_type;
    typedef unsigned char  unsigned_value_type;
    typedef unsigned char  value_type;
    typedef Unsigned       sign_type;
};

template<>
struct TraitsPrimitive<signed short>
{
    typedef signed   short signed_value_type;
    typedef unsigned short unsigned_value_type;
    typedef signed   short value_type;
    typedef Signed         sign_type;
};

template<>
struct TraitsPrimitive<unsigned short>
{
    typedef signed   short signed_value_type;
    typedef unsigned short unsigned_value_type;
    typedef unsigned short value_type;
    typedef Unsigned       sign_type;
};

template<>
struct TraitsPrimitive<signed int>
{
    typedef signed   int   signed_value_type;
    typedef unsigned int   unsigned_value_type;
    typedef signed   int   value_type;
    typedef Signed         sign_type;
};

template<>
struct TraitsPrimitive<unsigned int>
{
    typedef signed   int   signed_value_type;
    typedef unsigned int   unsigned_value_type;
    typedef unsigned int   value_type;
    typedef Unsigned       sign_type;
};

template<>
struct TraitsPrimitive<signed long>
{
    typedef signed   long  signed_value_type;
    typedef unsigned long  unsigned_value_type;
    typedef signed   long  value_type;
    typedef Signed         sign_type;
};

template<>
struct TraitsPrimitive<unsigned long>
{
    typedef signed   long  signed_value_type;
    typedef unsigned long  unsigned_value_type;
    typedef unsigned long  value_type;
    typedef Unsigned       sign_type;
};

template<typename T>
struct Traits
{
    typedef typename TraitsPrimitive<T>::signed_value_type   signed_value_type;
    typedef typename TraitsPrimitive<T>::unsigned_value_type unsigned_value_type;
    typedef typename TraitsPrimitive<T>::value_type          value_type;
    typedef typename TraitsPrimitive<T>::sign_type           sign_type;
    typedef value_type                                       arg_type;
    typedef const value_type                                 const_arg_type;
    typedef value_type&                                      ref_arg_type;
    typedef value_type                                       result_type;
    typedef const value_type                                 const_result_type;
    typedef unsigned_value_type                              mask_type;

    static const unsigned int Capacity = std::numeric_limits<unsigned_value_type>::digits;
};

template<int N>
struct Traits<MultiByte<N> >
{
    typedef MultiByte<N> multibyte;

    typedef typename multibyte::signed_value_type   signed_value_type;
    typedef typename multibyte::unsigned_value_type unsigned_value_type;
    typedef typename multibyte::value_type          value_type;
    typedef typename multibyte::sign_type           sign_type;
    typedef typename multibyte::arg_type            arg_type;
    typedef typename multibyte::const_arg_type      const_arg_type;
    typedef typename multibyte::ref_arg_type        ref_arg_type;
    typedef typename multibyte::result_type         result_type;
    typedef typename multibyte::const_result_type   const_result_type;
    typedef unsigned char                           mask_type;

    static const unsigned int Capacity = multibyte::Capacity;
};

//----------------------------------------------------------------------

template<int N, typename S = Unsigned>
struct Fit
{
    template<typename SIGN, bool LE_CHAR_SIZE, bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _
    {
        typedef Traits<MultiByte<N> > traits;
    };

    template<typename SIGN, bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<SIGN, true, LE_SHORT_SIZE, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<unsigned char> traits;
    };

    template<typename SIGN, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<SIGN, false, true, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<unsigned short> traits;
    };

    template<typename SIGN, bool LE_LONG_SIZE>
    struct _<SIGN, false, false, true, LE_LONG_SIZE>
    {
        typedef Traits<unsigned int> traits;
    };

    template<bool LE_SHORT_SIZE, bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<Signed, true, LE_SHORT_SIZE, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<signed char> traits;
    };

    template<bool LE_INT_SIZE, bool LE_LONG_SIZE>
    struct _<Signed, false, true, LE_INT_SIZE, LE_LONG_SIZE>
    {
        typedef Traits<signed short> traits;
    };

    template<bool LE_LONG_SIZE>
    struct _<Signed, false, false, true, LE_LONG_SIZE>
    {
        typedef Traits<signed int> traits;
    };

     typedef typename _< S,
                         (N <= Traits<unsigned char >::Capacity),
                         (N <= Traits<unsigned short>::Capacity),
                         (N <= Traits<unsigned int  >::Capacity),
                         (N <= Traits<unsigned long >::Capacity)
                       >::traits traits;

    typedef typename traits::signed_value_type   signed_value_type;
    typedef typename traits::unsigned_value_type unsigned_value_type;
    typedef typename traits::value_type          value_type;
    typedef typename traits::sign_type           sign_type;
    typedef typename traits::arg_type            arg_type;
    typedef typename traits::const_arg_type      const_arg_type;
    typedef typename traits::ref_arg_type        ref_arg_type;
    typedef typename traits::result_type         result_type;
    typedef typename traits::const_result_type   const_result_type;
};

template<typename T> struct TraitsType;
template<>           struct TraitsType<char>           { typedef Traits<char>           traits; };
template<>           struct TraitsType<signed char>    { typedef Traits<signed char>    traits; };
template<>           struct TraitsType<unsigned char>  { typedef Traits<unsigned char>  traits; };
template<>           struct TraitsType<signed short>   { typedef Traits<signed short>   traits; };
template<>           struct TraitsType<unsigned short> { typedef Traits<unsigned short> traits; };
template<>           struct TraitsType<signed int>     { typedef Traits<signed int>     traits; };
template<>           struct TraitsType<unsigned int>   { typedef Traits<unsigned int>   traits; };
template<>           struct TraitsType<signed long>    { typedef Traits<signed long>    traits; };
template<>           struct TraitsType<unsigned long>  { typedef Traits<unsigned long>  traits; };
template<int N>      struct TraitsType<MultiByte<N> >  { typedef Traits<MultiByte<N> >  traits; };

template<typename T, int N>
struct Mask
{
    // only declaration; for error message when invalid template parameter is used
    struct ERROR__INVALID_Bits_SIZE__ONLY_CAN_USE_FROM_ONE_TO_CONTAINER_DIGIT_SIZE;

    template<int M, bool GREATER_THAN_ZERO, bool LESS_THAN_M, bool EQUAL_M>
    struct _
    {
        static const T value = ERROR__INVALID_Bits_SIZE__ONLY_CAN_USE_FROM_ONE_TO_CONTAINER_DIGIT_SIZE::value;
    };

    template<int M>
    struct _<M, true, true, false>
    {
        static const T value = static_cast<T>((1u << M) - 1);
    };

    template<int M>
    struct _<M, true, false, true>
    {
        static const T value = static_cast<T>(-1);
    };

    template<int M>
    struct _<M, true, false, false>
    {
        static const T value = static_cast<T>((1u << (M % std::numeric_limits<unsigned char>::digits)) - 1);;
    };

    static const T value = _< N,
                              0 < N,
                              N < Traits<T>::Capacity,
                              N == Traits<T>::Capacity
                            >::value;

    static const T msb = value ^ (value >> 1);
};

template<int N>
struct Mask<Unsigned, N>
{
    static const unsigned char value = N % std::numeric_limits<unsigned char>::digits;
};

template<typename T, int SIZE, typename S>
struct Trimmer
{
    static const typename T::mask_type mask = Mask<typename T::mask_type, SIZE>::value;
    static const typename T::mask_type msb  = mask ^ (mask >> 1);

    static void trim(typename T::ref_arg_type n)
    {
        n &= mask;
        if((n & msb) != 0) // if MSB is high then ...
        {
            n |= ~mask;    // ... padding the left side with 1
        }
    }
};

template<typename T, int SIZE>
struct Trimmer<T, SIZE, Unsigned>
{
    static const typename T::mask_type mask = Mask<typename T::mask_type, SIZE>::value;

    static void trim(typename T::ref_arg_type n)
    {
        n &= mask;
    }
};

template<int N, int SIZE>
struct Trimmer<Traits<MultiByte<N> >, SIZE, Unsigned>
{
    typedef MultiByte<N> multibyte;

    static const typename multibyte::mask_type mask = Mask<typename multibyte::mask_type, SIZE>::value;

    static typename multibyte::const_result_type trim(typename multibyte::ref_arg_type n)
    {
        n.value_[0] &= mask;
        return n;
    }
};

template<int SIZE, typename T = typename Fit<SIZE>::value_type>
struct Container
{
    static const int Size = SIZE;

    template<int S, typename U> struct _              { typedef U                                        value_type; };
    template<int S>             struct _<S, Signed>   { typedef typename Fit<Size, Signed>::value_type   value_type; };
    template<int S>             struct _<S, Unsigned> { typedef typename Fit<Size, Unsigned>::value_type value_type; };

    typedef typename TraitsType<typename _<SIZE, T>::value_type>::traits traits;

    typedef typename traits::signed_value_type   signed_value_type;
    typedef typename traits::unsigned_value_type unsigned_value_type;
    typedef typename traits::value_type          value_type;
    typedef typename traits::sign_type           sign_type;
    typedef typename traits::arg_type            arg_type;
    typedef typename traits::const_arg_type      const_arg_type;
    typedef typename traits::ref_arg_type        ref_arg_type;
    typedef typename traits::result_type         result_type;
    typedef typename traits::const_result_type   const_result_type;
    typedef typename traits::mask_type           mask_type;

    static const int Capacity = traits::Capacity;

    static void trim(ref_arg_type n)
    {
        Trimmer<traits, Size, sign_type>::trim(n);
    }

    static const_result_type getSequence(const_arg_type value)
    {
        return static_cast<unsigned_value_type>(value) & Mask<mask_type, Size>::value;
    };
};

//----------------------------------------------------------------------

template<int SIZE, typename T>
class BitsBase
{
public:
    typedef Container<SIZE, T> container;

    typedef typename container::signed_value_type   signed_value_type;
    typedef typename container::unsigned_value_type unsigned_value_type;
    typedef typename container::value_type          value_type;
    typedef typename container::sign_type           sign_type;
    typedef typename container::arg_type            arg_type;
    typedef typename container::const_arg_type      const_arg_type;
    typedef typename container::ref_arg_type        ref_arg_type;
    typedef typename container::result_type         result_type;
    typedef typename container::const_result_type   const_result_type;

    static const int Size     = SIZE;
    static const int Capacity = container::Capacity;

    BitsBase() : value_()
    {
    }

    BitsBase(const_arg_type value) : value_(value)
    {
        trim(value_);
    }

    void set(const_arg_type value)
    {
        value_ = value;
        trim(value_);
    }

    const_result_type get() const
    {
        return value_;
    }

    static void trim(ref_arg_type n)
    {
        container::trim(n);
    }

    void setSequence(const_arg_type value)
    {
        value_ = value;
        trim(value_);
    }

    const_result_type getSequence() const
    {
        return container::getSequence(value_);
    }

private:
    value_type value_;
};

//----------------------------------------------------------------------

template<typename LHS, typename RHS>
class PackBase
{
public:
    static const int Size = LHS::Size + RHS::Size;

    typedef Container<Size> container;

    typedef typename container::value_type         value_type;
    typedef typename container::arg_type           arg_type;
    typedef typename container::const_arg_type     const_arg_type;
    typedef typename container::ref_arg_type       ref_arg_type;
    typedef typename container::result_type        result_type;
    typedef typename container::const_result_type  const_result_type;

    PackBase(LHS& lhs, RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
    }

    void setSequence(const_arg_type value)
    {
        rhs_.setSequence(value);
        lhs_.setSequence(value >> RHS::Size);
    }

    const_result_type getSequence() const
    {
        return (lhs_.getSequence() << RHS::Size) | rhs_.getSequence();
    }

private:
    LHS& lhs_;
    RHS& rhs_;
};

template<typename LHS, int N>
class PackBase<LHS, void (*)(Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef Container<Size> container;

    typedef typename container::value_type         value_type;
    typedef typename container::arg_type           arg_type;
    typedef typename container::const_arg_type     const_arg_type;
    typedef typename container::ref_arg_type       ref_arg_type;
    typedef typename container::result_type        result_type;
    typedef typename container::const_result_type  const_result_type;

    PackBase(LHS& lhs, void (*)(Reserved<N>*)) : lhs_(lhs)
    {
    }

    void setSequence(const_arg_type value)
    {
        lhs_.setSequence(value >> N);
    }

    const_result_type getSequence() const
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

    typedef Container<Size> container;

    typedef typename container::value_type         value_type;
    typedef typename container::arg_type           arg_type;
    typedef typename container::const_arg_type     const_arg_type;
    typedef typename container::ref_arg_type       ref_arg_type;
    typedef typename container::result_type        result_type;
    typedef typename container::const_result_type  const_result_type;

    ConstPackBase(const LHS& lhs, const RHS& rhs) : lhs_(lhs), rhs_(rhs)
    {
    }

    const_result_type getSequence() const
    {
        return (lhs_.getSequence() << RHS::Size) | rhs_.getSequence();
    }

private:
    const LHS& lhs_;
    const RHS& rhs_;
};

template<typename LHS, int N>
class ConstPackBase<LHS, void (*)(Reserved<N>*)>
{
public:
    static const int Size = LHS::Size + N;

    typedef Container<Size> container;

    typedef typename container::value_type         value_type;
    typedef typename container::arg_type           arg_type;
    typedef typename container::const_arg_type     const_arg_type;
    typedef typename container::ref_arg_type       ref_arg_type;
    typedef typename container::result_type        result_type;
    typedef typename container::const_result_type  const_result_type;

    ConstPackBase(const LHS& lhs, void (*)(Reserved<N>*)) : lhs_(lhs)
    {
    }

    const_result_type getSequence() const
    {
        return lhs_.getSequence() << N;
    }

private:
    const LHS& lhs_;
};

//----------------------------------------------------------------------

template<typename T, typename U>
struct Sign
{
    template<bool C, typename T1, typename T2> struct _                { typedef T1 type; };
    template<typename T1, typename T2>         struct _<false, T1, T2> { typedef T2 type; };

    typedef typename _<std::numeric_limits<T>::is_signed && std::numeric_limits<U>::is_signed, Signed, Unsigned>::type sign_type;
};

template<typename T>
struct Sign<T, Unsigned>
{
    typedef Unsigned sign_type;
};

template<typename U>
struct Sign<Unsigned, U>
{
    typedef Unsigned sign_type;
};

template<>
struct Sign<Unsigned, Unsigned>
{
    typedef Unsigned sign_type;
};

template<>
struct Sign<Signed, Signed>
{
    typedef Signed sign_type;
};

template<int N, int M, typename T, typename U>
struct Result
{
    static const int Size = (N < M) ? M : N;

    typedef typename Sign<T, U>::sign_type sign_type;

    typedef Bits<Size, typename detail::Fit<Size, sign_type>::value_type> result_type;
};

//----------------------------------------------------------------------

} // namespace detail

//----------------------------------------------------------------------

template<int SIZE, typename T = Unsigned>
class Bits : private detail::BitsBase<SIZE, T>
{
public:
    typedef detail::BitsBase<SIZE, T> super;

    typedef typename super::signed_value_type   signed_value_type;
    typedef typename super::unsigned_value_type unsigned_value_type;
    typedef typename super::value_type          value_type;
    typedef typename super::sign_type           sign_type;
    typedef typename super::arg_type            arg_type;
    typedef typename super::const_arg_type      const_arg_type;
    typedef typename super::ref_arg_type        ref_arg_type;
    typedef typename super::result_type         result_type;
    typedef typename super::const_result_type   const_result_type;

    static const int Size     = super::Size;
    static const int Capacity = super::Capacity;

    static int size()
    {
        return Size;
    }

    Bits() : super()
    {
    }

    explicit Bits(const_arg_type n) : super(n)
    {
    }

    template<int M, typename U>
    explicit Bits(const Bits<M, U>& bits) : super(bits.get())
    {
    }

    Bits& set(const_arg_type n)
    {
        super::set(n);
        return *this;
    }

    const_result_type get() const
    {
        return super::get();
    }

    void setSequence(const_arg_type value)
    {
        super::setSequence(value);
    }

    const_result_type getSequence() const
    {
        return super::getSequence();
    }

    operator const_result_type () const
    {
        return get();
    }

    const Bits& operator + () const
    {
        return *this;
    }

    Bits operator - () const
    {
        return Bits(-super::get());
    }

    Bits operator ~ () const
    {
        return Bits(~super::get());
    }

    Bits& operator ++ ()
    {
        *this += 1;
        return *this;
    }

    Bits operator ++ (int)
    {
        Bits result(*this);
        ++*this;
        return result;
    }

    Bits& operator -- ()
    {
        *this -= 1;
        return *this;
    }

    Bits operator -- (int)
    {
        Bits result(*this);
        --*this;
        return result;
    }

    Bits& operator = (const_arg_type n)
    {
        return set(n);
    }

    Bits& operator += (const_arg_type n)
    {
        return set(super::get() + n);
    }

    Bits& operator -= (const_arg_type n)
    {
        return set(super::get() - n);
    }

    Bits& operator *= (const_arg_type n)
    {
        return set(super::get() * n);
    }

    Bits& operator /= (const_arg_type n)
    {
        return set(super::get() / n);
    }

    Bits& operator %= (const_arg_type n)
    {
        return set(super::get() % n);
    }

    Bits& operator |= (const_arg_type n)
    {
        return set(super::get() | n);
    }

    Bits& operator &= (const_arg_type n)
    {
        return set(super::get() & n);
    }

    Bits& operator ^= (const_arg_type n)
    {
        return set(super::get() ^ n);
    }

    Bits& operator <<= (int n)
    {
        super::set(super::get() << n);
        return *this;
    }

    Bits& operator >>= (int n)
    {
        super::set(super::get() >> n);
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

//----------------------------------------------------------------------

template<typename LHS, typename RHS> class ConstPack;

template<typename LHS, typename RHS>
class Pack : public detail::PackBase<LHS, RHS>
{
public:
    typedef detail::PackBase<LHS, RHS> super;

    static const int Size = super::Size;

    typedef typename super::value_type         value_type;
    typedef typename super::arg_type           arg_type;
    typedef typename super::const_arg_type     const_arg_type;
    typedef typename super::ref_arg_type       ref_arg_type;
    typedef typename super::result_type        result_type;
    typedef typename super::const_result_type  const_result_type;

    static int size()
    {
        return Size;
    }

    Pack(LHS& lhs, RHS& rhs) : super(lhs, rhs)
    {
    }

    Pack& operator = (const_arg_type value)
    {
        setSequence(value);
        return *this;
    }

    Pack& operator = (const Pack& value)
    {
        setSequence(value.getSequence());
        return *this;
    }

    operator result_type () const
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

    typedef typename super::value_type         value_type;
    typedef typename super::arg_type           arg_type;
    typedef typename super::const_arg_type     const_arg_type;
    typedef typename super::ref_arg_type       ref_arg_type;
    typedef typename super::result_type        result_type;
    typedef typename super::const_result_type  const_result_type;

    static int size()
    {
        return Size;
    }

    ConstPack(const LHS& lhs, const RHS& rhs) : super(lhs, rhs)
    {
    }

    operator result_type () const
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

//----------------------------------------------------------------------

#define EMATTSAN_BITS_DEFINE_OP(op)                                                            \
                                                                                               \
template<int N, typename T, int M, typename U>                                                 \
inline typename detail::Result<N, M, T, U>::result_type                                        \
operator op (const Bits<N, T>& lhs, const Bits<M, U>& rhs)                                     \
{                                                                                              \
    return typename detail::Result<N, M, T, U>::result_type(lhs.get() op rhs.get());           \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, typename Bits<N, T>::signed_value_type>::result_type   \
operator op (const Bits<N, T>& lhs, typename Bits<N, T>::signed_value_type rhs)                \
{                                                                                              \
    return lhs op Bits<N, typename Bits<N, T>::signed_value_type>(rhs);                        \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, typename Bits<N, T>::signed_value_type>::result_type   \
operator op (typename Bits<N, T>::signed_value_type lhs, const Bits<N, T>& rhs)                \
{                                                                                              \
    return Bits<N, typename Bits<N, T>::signed_value_type>(lhs) op rhs;                        \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, typename Bits<N, T>::unsigned_value_type>::result_type \
operator op (const Bits<N, T>& lhs, typename Bits<N, T>::unsigned_value_type rhs)              \
{                                                                                              \
    return lhs op Bits<N, typename Bits<N, T>::unsigned_value_type>(rhs);                      \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, typename Bits<N, T>::unsigned_value_type>::result_type \
operator op (typename Bits<N, T>::unsigned_value_type lhs, const Bits<N, T>& rhs)              \
{                                                                                              \
    return Bits<N, typename Bits<N, T>::unsigned_value_type>(lhs) op rhs;                      \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, signed int>::result_type                               \
operator op (const Bits<N, T>& lhs, signed int rhs)                                            \
{                                                                                              \
    return lhs op Bits<N, typename Bits<N, T>::signed_value_type>(rhs);                        \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, signed int>::result_type                               \
operator op (signed int lhs, const Bits<N, T>& rhs)                                            \
{                                                                                              \
    return Bits<N, typename Bits<N, T>::signed_value_type>(lhs) op rhs;                        \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, unsigned int>::result_type                             \
operator op (const Bits<N, T>& lhs, unsigned int rhs)                                          \
{                                                                                              \
    return lhs op Bits<N, typename Bits<N, T>::unsigned_value_type>(rhs);                      \
}                                                                                              \
                                                                                               \
template<int N, typename T>                                                                    \
inline typename detail::Result<N, N, T, unsigned int>::result_type                             \
operator op (unsigned int lhs, const Bits<N, T>& rhs)                                          \
{                                                                                              \
    return Bits<N, typename Bits<N, T>::unsigned_value_type>(lhs) op rhs;                      \
}


EMATTSAN_BITS_DEFINE_OP(+)
EMATTSAN_BITS_DEFINE_OP(-)
EMATTSAN_BITS_DEFINE_OP(*)
EMATTSAN_BITS_DEFINE_OP(/)
EMATTSAN_BITS_DEFINE_OP(%)
EMATTSAN_BITS_DEFINE_OP(|)
EMATTSAN_BITS_DEFINE_OP(&)
EMATTSAN_BITS_DEFINE_OP(^)

#undef EMATTSAN_BITS_DEFINE_OP

//----------------------------------------------------------------------

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

//----------------------------------------------------------------------

template<int N> void reserve(detail::Reserved<N>*) {}

//----------------------------------------------------------------------

} // namespace bits

//----------------------------------------------------------------------

} // namespace emattsan

//----------------------------------------------------------------------

#endif//EMATTSAN_BITS_H
