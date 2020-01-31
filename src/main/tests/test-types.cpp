#include "y/type/ints.hpp"
#include "y/type/fourcc.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;

namespace {
    template <typename A,typename B>
    static inline void test_select()
    {
        const std::type_info &a = typeid(A);
        const std::type_info &b = typeid(B);
        typedef typename select_type<true, A,B>::result sa_type;
        typedef typename select_type<false,A,B>::result sb_type;
        Y_CHECK(typeid(sa_type)==a);
        Y_CHECK(typeid(sb_type)==b);

    }

    template <typename T>
    static inline void test_ints()
    {
        std::cerr << typeid(T).name() << std::endl;
        std::cerr << "\tmin=" << limit_of<T>::minimum << std::endl;
        std::cerr << "\tmax=" << limit_of<T>::maximum << std::endl;
    }

    static inline void testFOURCC()
    {
        for(size_t iter=0;iter<1024;++iter)
        {
            const unsigned a0 = alea.full<uint8_t>();
            const unsigned a1 = alea.full<uint8_t>();
            const unsigned a2 = alea.full<uint8_t>();
            const unsigned a3 = alea.full<uint8_t>();
            const uint32_t ff = Y_FOURCC(a0,a1,a2,a3);

            const unsigned b0 = Y_FOURCC_AT(0,ff);
            const unsigned b1 = Y_FOURCC_AT(1,ff);
            const unsigned b2 = Y_FOURCC_AT(2,ff);
            const unsigned b3 = Y_FOURCC_AT(3,ff);

            Y_ASSERT(b0==a0);
            Y_ASSERT(b1==a1);
            Y_ASSERT(b2==a2);
            Y_ASSERT(b3==a3);
        }

        for(size_t iter=0;iter<8;++iter)
        {
            const unsigned a0    = alea.range<char>('a','z');
            const unsigned a1    = alea.range<char>('A','Z');
            const unsigned a2    = alea.range<char>('0','9');
            const unsigned a3    = alea.range<char>('a','z');
            const uint32_t dw    = Y_FOURCC(a0,a1,a2,a3);
            const char     id[8] = Y_FOURCC_CHAR8(dw);

            fprintf(stderr, "0x%02x => '%s' / '%s' \n", dw, fourcc_(dw), id);

        }
    }

}


Y_UTEST(types)
{
    test_select<double,int>();
    test_ints<short>();
    test_ints<unsigned short>();
    test_ints<int>();
    test_ints<unsigned int>();
    test_ints<long>();
    test_ints<unsigned long>();
    const uint32_t t1 = Y_FOURCC(1,2,3,4);
    const uint32_t t2 = Y_FOURCC(4,3,2,1);
    std::cerr << std::hex << t1 << " | " << t2 << std::endl;

    testFOURCC();

}
Y_UTEST_DONE()
