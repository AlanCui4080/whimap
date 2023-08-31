#include "database.hpp"
#include <hwy/print-inl.h>
#include <iostream>
enum
{
    ERR_RUNTIME_NOT_SATISFIED,
    ERR_INCORRECT_ADD,
    ERR_INCORRECT_SUB,
    ERR_INCORRECT_MUL,
    ERR_INCORRECT_DIV,
    ERR_INCORRECT_AND,
    ERR_INCORRECT_OR,
};
template <typename S, bool Enable_MulDiv = std::is_floating_point_v<S>>
class test
{
public:
    int run()
    {
        auto              p1 = whimap::simd::AllocateAligned<S>(64);
        auto              p2 = whimap::simd::AllocateAligned<S>(64);
        whimap::expvec<S> a(std::move(p1), 64);
        whimap::expvec<S> b(std::move(p2), 64);
        constexpr auto    slen = 64;
        constexpr auto    na   = std::numeric_limits<S>::max() / 2;
        constexpr auto    lna  = std::numeric_limits<S>::max() / 20;
        constexpr auto    nb   = std::numeric_limits<S>::max() / 3;
        constexpr auto    lnb  = std::numeric_limits<S>::max() / 30;
        constexpr auto    vlen = slen / whimap::simd::Lanes(a.D_tag);
        using vtype            = whimap::expvec<S>::vector_type;
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = a[i];
            v        = whimap::simd::Set(a.D_tag, na);
        }
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = b[i];
            v        = whimap::simd::Set(b.D_tag, nb);
        }
        auto c = a + b;
        auto d = a - b;
        auto e = a | b;
        auto f = a & b;
        std::cout << std::hex << "Excepted: " << na + nb << std::endl;
        whimap::simd::Print(c.D_tag, "", c[0]);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    c.D_tag, c[i] == whimap::simd::Set(c.D_tag, na + nb)))
                return -ERR_INCORRECT_ADD;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(d.D_tag, "", d[0]);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    d.D_tag, d[i] == whimap::simd::Set(d.D_tag, na - nb)))
                return -ERR_INCORRECT_SUB;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(e.D_tag, "", e[0]);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    e.D_tag, e[i] == whimap::simd::Set(e.D_tag, na | nb)))
                return -ERR_INCORRECT_AND;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(f.D_tag, "", f[0]);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    f.D_tag, f[i] == whimap::simd::Set(f.D_tag, na & nb)))
                return -ERR_INCORRECT_OR;
        }
        return 0;
    }
};
template <typename S> class test<S, true>
{
public:
    int run()
    {
        auto              p1 = whimap::simd::AllocateAligned<S>(64);
        auto              p2 = whimap::simd::AllocateAligned<S>(64);
        whimap::expvec<S> a(std::move(p1), 64);
        whimap::expvec<S> b(std::move(p2), 64);
        constexpr auto    slen = 64;
        constexpr auto    na   = std::numeric_limits<S>::max() / 2;
        constexpr auto    lna  = std::numeric_limits<S>::max() / 2000;
        constexpr auto    nb   = std::numeric_limits<S>::max() / 3;
        constexpr auto    lnb  = std::numeric_limits<S>::max() / 3000;
        constexpr auto    vlen = slen / whimap::simd::Lanes(a.D_tag);
        using vtype            = whimap::expvec<S>::vector_type;
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = a[i];
            v        = whimap::simd::Set(a.D_tag, na);
        }
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = b[i];
            v        = whimap::simd::Set(b.D_tag, nb);
        }
        auto c = a + b;
        auto d = a - b;

        std::cout << std::hex << "Excepted: " << na + nb << std::endl;
        whimap::simd::Print(c.D_tag, "", c[0]);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    c.D_tag, c[i] == whimap::simd::Set(c.D_tag, na + nb)))
                return -ERR_INCORRECT_ADD;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(d.D_tag, "", d[0]);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    d.D_tag, d[i] == whimap::simd::Set(d.D_tag, na - nb)))
                return -ERR_INCORRECT_SUB;
        }

        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = a[i];
            v        = whimap::simd::Set(a.D_tag, lna);
        }
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = b[i];
            v        = whimap::simd::Set(b.D_tag, lnb);
        }
        auto e = a * b;
        auto f = a / b;
        std::cout << std::hex << "Excepted: " << lna * lnb << std::endl;
        whimap::simd::Print(e.D_tag, "", e[0]);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    e.D_tag, e[i] == whimap::simd::Set(e.D_tag, lna * lnb)))
                return -ERR_INCORRECT_MUL;
        }
        std::cout << std::hex << "Excepted: " << lna / lnb << std::endl;
        whimap::simd::Print(f.D_tag, "", f[0]);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    f.D_tag, f[i] == whimap::simd::Set(f.D_tag, lna / lnb)))
                return -ERR_INCORRECT_DIV;
        }
        return 0;
    }
};
int main()
{
    if ((HWY_STATIC_TARGET & HWY_AVX2))
    {
        return -ERR_RUNTIME_NOT_SATISFIED;
    }
#define TEST_SIMD(x)                     \
    [[maybe_unused]] int _impl__rz##x;   \
    test<x>              _impl__##x;     \
    if (_impl__rz##x = _impl__##x.run()) \
    {                                    \
        return _impl__rz##x;             \
    }                                    \
    _impl__rz##x

    TEST_SIMD(int8_t);
    TEST_SIMD(int16_t);
    TEST_SIMD(int32_t);
    TEST_SIMD(int64_t);
    TEST_SIMD(uint8_t);
    TEST_SIMD(uint16_t);
    TEST_SIMD(uint32_t);
    TEST_SIMD(uint64_t);
    TEST_SIMD(float);
    TEST_SIMD(double);
    return 0;
}
