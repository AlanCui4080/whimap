#include <iostream>
#include <limits>
#include <stdfloat>
#include "database.hpp"
#include "simd-utility.hpp"
#include <hwy/print-inl.h>
enum
{
    ERR_RUNTIME_NOT_SATISFIED,
    ERR_INCORRECT_ADD,
    ERR_INCORRECT_SUB,
    ERR_INCORRECT_MUL,
    ERR_INCORRECT_DIV,
    ERR_INCORRECT_AND,
    ERR_INCORRECT_OR,
    ERR_INCORRECT_LS,
    ERR_INCORRECT_RS,
};
template <typename S, bool Enable_MulDiv = std::is_floating_point_v<S>>
class test
{
public:
    int run()
    {
        auto              p1 = whimap::simd::AllocateAligned<S>(64);
        auto              p2 = whimap::simd::AllocateAligned<S>(64);
        auto              p3 = whimap::simd::AllocateAligned<S>(64);
        whimap::expvec<S> a(std::move(p1), 64);
        whimap::expvec<S> b(std::move(p2), 64);
        whimap::expvec<S> yc(std::move(p3), 64);
        auto              slen = 64;
        auto              na   = std::numeric_limits<S>::max() / 2;
        auto              nb   = std::numeric_limits<S>::max() / 3;
        auto              vlen = slen / whimap::simd::Lanes(a.D_tag);
        using vtype            = typename whimap::expvec<S>::vector_type;
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
        auto g = a ^ b;
        for (size_t i = 0; i < vlen; i++)
        {
            vtype& v = yc[i];
            v        = whimap::simd::Set(yc.D_tag, nb % 8);
        }
        auto h = a >> yc;
        auto u = a << yc;
        std::cout << std::hex << "Excepted: " << na + nb << std::endl;
        whimap::simd::Print(c.D_tag, "ADD", c[0], 0, 32);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    c.D_tag, c[i] == whimap::simd::Set(c.D_tag, na + nb)))
                return -ERR_INCORRECT_ADD;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(d.D_tag, "SUB", d[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    d.D_tag, d[i] == whimap::simd::Set(d.D_tag, na - nb)))
                return -ERR_INCORRECT_SUB;
        }
        std::cout << std::hex << "Excepted: " << (na | nb) << std::endl;
        whimap::simd::Print(e.D_tag, "AND", e[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    e.D_tag, e[i] == whimap::simd::Set(e.D_tag, na | nb)))
                return -ERR_INCORRECT_AND;
        }
        std::cout << std::hex << "Excepted: " << (na & nb) << std::endl;
        whimap::simd::Print(f.D_tag, "OR", f[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    f.D_tag, f[i] == whimap::simd::Set(f.D_tag, na & nb)))
                return -ERR_INCORRECT_OR;
        }
        std::cout << std::hex << "Excepted: " << (na >> nb % 8) << std::endl;
        whimap::simd::Print(e.D_tag, "LS", e[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    h.D_tag, h[i] == whimap::simd::Set(h.D_tag, na >> nb % 8)))
                return -ERR_INCORRECT_LS;
        }
        std::cout << std::hex << "Excepted: " << (na << (nb % 8)) << std::endl;
        whimap::simd::Print(f.D_tag, "RS", f[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    u.D_tag,
                    u[i] == whimap::simd::Set(u.D_tag, na << (nb % 8))))
                return -ERR_INCORRECT_RS;
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
        auto              p3 = whimap::simd::AllocateAligned<S>(64);
        whimap::expvec<S> a(std::move(p1), 64);
        whimap::expvec<S> b(std::move(p2), 64);
        whimap::expvec<S> yc(std::move(p3), 64);
        auto              slen = 64;
        auto              na   = std::numeric_limits<S>::max() / 2;
        auto              nb   = std::numeric_limits<S>::max() / 3;
        auto              vlen = slen / whimap::simd::Lanes(a.D_tag);
        using vtype            = typename whimap::expvec<S>::vector_type;
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
        auto e = a * b;
        auto f = a / b;
        auto c = a + b;
        auto d = a - b;
        std::cout << std::hex << "Excepted: " << na + nb << std::endl;
        whimap::simd::Print(c.D_tag, "ADD", c[0], 0, 32);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    c.D_tag, c[i] == whimap::simd::Set(c.D_tag, na + nb)))
                return -ERR_INCORRECT_ADD;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(d.D_tag, "SUB", d[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    d.D_tag, d[i] == whimap::simd::Set(d.D_tag, na - nb)))
                return -ERR_INCORRECT_SUB;
        }
        std::cout << std::hex << "Excepted: " << na * nb << std::endl;
        whimap::simd::Print(e.D_tag, "MUL", e[0], 0, 32);
        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    e.D_tag, e[i] == whimap::simd::Set(e.D_tag, na * nb)))
                return -ERR_INCORRECT_MUL;
        }
        std::cout << std::hex << "Excepted: " << na - nb << std::endl;
        whimap::simd::Print(f.D_tag, "DIV", f[0], 0, 32);

        for (size_t i = 0; i < vlen; i++)
        {
            if (!whimap::simd::AllTrue(
                    f.D_tag, f[i] == whimap::simd::Set(f.D_tag, na / nb)))
                return -ERR_INCORRECT_DIV;
        }
        return 0;
    }
};
int main()
{
    if (!(HWY_SUPPORTED_TARGETS & HWY_AVX2))
    {
        std::cout << "Reqiured Following Instructions But not found:"
                  << std::endl;
        //-mavx2 -mavx -msha -maes -mbmi -mbmi2 -mfma -mf16c -mpclmul
        std::cout << HWY_TARGET_STR_AVX2 << std::endl;
        exit(EXIT_FAILURE);
    }
    if (HWY_HAVE_RUNTIME_DISPATCH)
        std::cout << "Runtime Dispatched." << std::endl;
    std::cout << "Supported Targets:" << std::endl;
    for (auto& i : hwy::SupportedAndGeneratedTargets())
    {
        std::cout << hwy::TargetName(i) << std::endl;
    }

#define TEST_SIMD(x)                                                    \
    test<x> _impl__##x;                                                 \
    int     _impl__rz##x = _impl__##x.run();                            \
    if ((_impl__rz##x) != 0)                                            \
    {                                                                   \
        std::cout << std::dec << "ERRCODE : " << (signed)(_impl__rz##x) \
                  << std::endl;                                         \
        exit(EXIT_FAILURE);                                             \
    }
    TEST_SIMD(int8_t);
    TEST_SIMD(int16_t);
    TEST_SIMD(int32_t);
    TEST_SIMD(int64_t);
    TEST_SIMD(uint8_t);
    TEST_SIMD(uint16_t);
    TEST_SIMD(uint32_t);
    TEST_SIMD(uint64_t);
    // removed fps, we dont really need it and it will broken things
    // recovery it back
    using namespace std;
    
    TEST_SIMD(float);
    TEST_SIMD(double);
    return 0;
}
