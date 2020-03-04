#include "y/type/memops.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"

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


    static inline bool isZero_(const void *addr, const size_t size) throw()
    {
        const uint8_t *p = static_cast<const uint8_t *>(addr);
        for(size_t i=0;i<size;++i)
        {
            if(p[i]!=0) return false;
        }
        return true;
    }

    template <typename T>
    static inline bool isZero( const T &tmp ) throw()
    {
        return isZero_( &tmp, sizeof(tmp) );
    }


    template <typename T> static inline
    void testZero()
    {
        typedef core::memops<sizeof(T)> ops;
        typedef typename ops::word_type word_type;
        std::cerr << "\tzero <" << type_name_of<T>() << ">, size=" << sizeof(T) << " -> " << type_name_of<word_type>() << "/" << ops::num_words << std::endl;
        {
            T tmp;
            alea.fill( &tmp, sizeof(tmp) );
            while(isZero(tmp)) alea.fill(&tmp,sizeof(tmp));
            memops::zero(tmp);
            Y_CHECK(isZero(tmp));
        }
    }

    static inline void testZeros()
    {
        std::cerr << "-- testing zeros" << std::endl;
        testZero<char>();
        testZero<int16_t>();
        testZero<int32_t>();
        testZero<int64_t>();
        testZero<double>();
        testZero<float>();
        testZero< complex<float>  >();
        testZero< complex<double> >();
        testZero< point2d<char>   >();
        testZero< point3d<char>   >();
        testZero< point3d<int16_t> >();
    }

}

Y_UTEST(memops)
{
    testTypes();
    testZeros();
}
Y_UTEST_DONE()

