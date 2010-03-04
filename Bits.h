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

} // namespace detail

template<class LHS, class RHS> class Pack;
template<class LHS, class RHS> class ConstPack;

template<typename T>
class Bits
{
public:
    typedef typename detail::Traits<T>::signed_value_type   signed_value_type;
    typedef typename detail::Traits<T>::unsigned_value_type unsigned_value_type;

    // member functions of Bits

    virtual ~Bits() {}
    virtual int size() const = 0;
    virtual void setSequence(unsigned_value_type value) = 0;
    virtual unsigned_value_type getSequence() const = 0;

    template<class RHS>
    Pack<T, RHS> operator , (Bits<RHS>& rhs) { return Pack<T, RHS>(*this, rhs); }

    Pack<T, int(*)()> operator , (int (*rhs)()) { return Pack<T, int(*)()>(*this, rhs); }

    template<class RHS>
    ConstPack<T, RHS> operator , (const Bits<RHS>& rhs) const { return ConstPack<T, RHS>(*this, rhs); }

    ConstPack<T, int(*)()> operator , (int(*rhs)()) const { return ConstPack<T, int(*)()>(*this, rhs); }
};

template<int SIZE, typename T = unsigned int>
class Signed : public Bits<T>
{
public:
    typedef typename Bits<T>::signed_value_type   signed_value_type;
    typedef typename Bits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Bits<T>::signed_value_type   value_type;

    static const int Size = SIZE;

    explicit Signed(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    Signed& set(value_type n)
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

    Signed& operator = (value_type n) { return set(n); }

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
class Unsigned : public Bits<T>
{
public:
    typedef typename Bits<T>::signed_value_type   signed_value_type;
    typedef typename Bits<T>::unsigned_value_type unsigned_value_type;
    typedef typename Bits<T>::unsigned_value_type value_type;

    static const int Size = SIZE;

    explicit Unsigned(value_type n = 0) : value_(trim(n)) {}

    int size() const { return Size; }

    Unsigned& set(value_type n)
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

    Unsigned& operator = (value_type n) { return set(n); }

    operator value_type () const { return get(); }

private:
    value_type trim(value_type n)
    {
        return n & ((1u << Size) - 1);
    }

    value_type value_;
};

template<class LHS, class RHS>
class Pack : public Bits<long>
{
public:
    typedef Bits<long>::unsigned_value_type unsigned_value_type;

    Pack(Bits<LHS>& lhs, Bits<RHS>& rhs) : lhs_(lhs), rhs_(rhs), size_(lhs.size() + rhs.size()) {}

    int size() const { return size_; }

    void setSequence(unsigned_value_type value)
    {
        rhs_.setSequence(value);
        lhs_.setSequence(value >> rhs_.size());
    }

    unsigned_value_type getSequence() const
    {
        return (lhs_.getSequence() << rhs_.size()) | rhs_.getSequence();
    }

    template<class R>
    Pack<long, R> operator , (Bits<R>& rhs) { return Pack<long, R>(*this, rhs); }

    template<class R>
    ConstPack<long, R> operator , (const Bits<R>& rhs) const { return ConstPack<long, R>(*this, rhs); }

    Pack& operator = (unsigned_value_type value) { setSequence(value); return *this; }

    operator unsigned_value_type () const { return getSequence(); }

private:
    Bits<LHS>& lhs_;
    Bits<RHS>& rhs_;
    const int  size_;
};

template<class LHS>
class Pack<LHS, int(*)()> : public Bits<long>
{
public:
    typedef Bits<long>::unsigned_value_type unsigned_value_type;

    Pack(Bits<LHS>& lhs, int (*rhs)()) : lhs_(lhs), rhs_(rhs) {}

    int size() const { return lhs_.size() + rhs_(); }

    void setSequence(unsigned_value_type value)
    {
        lhs_.setSequence(value >> rhs_());
    }

    unsigned_value_type getSequence() const
    {
        return lhs_.getSequence() << rhs_();
    }

    template<class R>
    Pack<long, R> operator , (Bits<R>& rhs) { return Pack<long, R>(*this, rhs); }

    template<class R>
    ConstPack<long, R> operator , (const Bits<R>& rhs) const { return ConstPack<long, R>(*this, rhs); }

    Pack& operator = (unsigned_value_type value) { setSequence(value); return *this; }

    operator unsigned_value_type () const { return getSequence(); }

private:
    Bits<LHS>& lhs_;
    int (*rhs_)();
};

template<class LHS, class RHS>
class ConstPack : public Bits<long>
{
public:
    typedef Bits<long>::unsigned_value_type unsigned_value_type;

    ConstPack(const Bits<LHS>& lhs, const Bits<RHS>& rhs) : lhs_(lhs), rhs_(rhs), size_(lhs.size() + rhs.size()) {}

    int size() const { return size_; }

    void setSequence(unsigned_value_type) { /* nop: This function is not used */ }

    unsigned_value_type getSequence() const
    {
        return (lhs_.getSequence() << rhs_.size()) | rhs_.getSequence();
    }

    template<class R>
    ConstPack<long, R> operator , (Bits<R>& rhs) const { return ConstPack<long, R>(*this, rhs); }

    operator unsigned_value_type () const { return getSequence(); }

private:
    const Bits<LHS>& lhs_;
    const Bits<RHS>& rhs_;
    const int        size_;
};

template<class LHS>
class ConstPack<LHS, int(*)()> : public Bits<long>
{
public:
    typedef Bits<long>::unsigned_value_type unsigned_value_type;

    ConstPack(const Bits<LHS>& lhs, int (*rhs)()) : lhs_(lhs), rhs_(rhs) {}

    int size() const { return lhs_.size() + rhs_(); }

    void setSequence(unsigned_value_type) { /* nop: This function is not used */ }

    unsigned_value_type getSequence() const
    {
        return lhs_.getSequence() << rhs_();
    }

    template<class R>
    ConstPack<long, R> operator , (Bits<R>& rhs) const { return ConstPack<long, R>(*this, rhs); }

    operator unsigned_value_type () const { return getSequence(); }

private:
    const Bits<LHS>& lhs_;
    int (*rhs_)();
};

template<int N>
inline int reserve() { return N; }

} // namespace bits

} // namespace emattsan

#endif//EMATTSAN_BITS_H
