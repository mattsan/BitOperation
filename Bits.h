#ifndef BITS_H
#define BITS_H

namespace emattsan
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

} // namespace detail

template<typename T>
class Bits
{
public:
    typedef typename detail::Traits<T>::signed_value_type   signed_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type unsigned_value_type;

    template<class LHS, class RHS>
    class Pack
    {
    public:
        typedef Pack<LHS, RHS> this_type;

        typedef detail::Traits<long>::unsigned_value_type unsigned_value_type;

        Pack(LHS& lhs, RHS& rhs) : lhs_(lhs), rhs_(rhs), size_(lhs.size() + rhs.size()) {}

        int size() const { return size_; }

        Pack& setSequence(unsigned_value_type value)
        {
            rhs_.setSequence(value);
            lhs_.setSequence(value >> rhs_.size());
            return *this;
        }

        unsigned_value_type getSequence() const
        {
            return (lhs_.getSequence() << rhs_.size()) | rhs_.getSequence();
        }

        template<class R>
        Pack<this_type, R> operator , (R& rhs) { return Pack<this_type, R>(*this, rhs); }

        Pack& operator = (unsigned_value_type value) { return setSequence(value); }

        operator unsigned_value_type () const { return getSequence(); }

    private:
        LHS&      lhs_;
        RHS&      rhs_;
        const int size_;
    };

    // member functions of Bits

    virtual ~Bits() {}
    virtual int size() const = 0;
    virtual void setSequence(unsigned_value_type value) = 0;
    virtual unsigned_value_type getSequence() const = 0;

    template<class RHS>
    Pack<Bits, RHS> operator , (RHS& rhs) { return Pack<Bits, RHS>(*this, rhs); }

    template<class RHS>
    const Pack<const Bits, const RHS> operator , (const RHS& rhs) const { return Pack<const Bits, const RHS>(*this, rhs); }
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
