#include "y/type/memops.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
using namespace upsylon;

namespace {

    static const size_t ILOG_MAX = ilog2<sizeof(uint64_t)>::value;

    template <size_t N>
    static inline void testType()
    {
        typedef typename core::memcull<N,ILOG_MAX>::word_type word_t;
        std::cerr << "for " << N << ": " << type_name_of<word_t>() << std::endl;
    }

    static inline void testTypes()
    {
        std::cerr << "-- testing types" << std::endl;
        testType<1>();
        testType<2>();
        testType<3>();
        testType<4>();
        testType<5>();
        testType<6>();
        testType<7>();
        testType<8>();
        testType<9>();
        testType<10>();
        testType<11>();
        testType<12>();
        testType<96>();
    }

    template <typename T> static inline
    void testZero()
    {
        {
            T tmp;
            alea.fill( &tmp, sizeof(tmp) );
            memops::zero(tmp);
        }
    }

    static inline void testZeros()
    {
        testZero<char>();
        testZero<int16_t>();
        testZero<int32_t>();
        testZero<int64_t>();
        testZero<double>();
    }

}

Y_UTEST(memops)
{
    testTypes();
    testZeros();
}
Y_UTEST_DONE()

