
#include "y/type/ints-to-real.hpp"
#include "y/utest/run.hpp"
#include <cfloat>

using namespace upsylon;

template <typename REAL>
bool test_i2r( const uint64_t u );

template <>
inline bool test_i2r<float>( const uint64_t u )
{
    const float    f = u;
    const uint64_t n = f;
    if(u!=n)
    {
        std::cerr << u << "->" << f << "->" << n << std::endl;
    }
    return u==n;
}

template <>
inline bool test_i2r<double>( const uint64_t u )
{
    const double    f = u;
    const uint64_t  n = f;
    if(u!=n)
    {
        std::cerr << u << "->" << f << "->" << n << std::endl;
    }
    return u==n;
}

template <typename REAL>
static inline uint64_t find_i2r(uint64_t u)
{
    while(u< limit_of<uint64_t>::maximum && test_i2r<REAL>(u) )
    {
        ++u;
    }
    
    return u;
}


#define TEST_I2R(U) \
do { if( !test_i2r(U) )  std::cerr << "overflow" << std::endl; } while(false)

Y_UTEST(i2r)
{
    
    
    std::cerr << FLT_MANT_DIG << std::endl;
    std::cerr << DBL_MANT_DIG << std::endl;
    std::cerr << LDBL_MANT_DIG << std::endl;

    std::cerr << std::endl;
    
    const uint64_t uf = find_i2r<float>(0);
    std::cerr << "i2r<float>  = " << uf << ", bits=" <<   bits_for(uf) << std::endl;
    
    const uint64_t ud = find_i2r<double>( (uint64_t(1) << 53) - 10000000 );
    std::cerr << "i2r<double> = " << ud << ", bits=" << bits_for(ud) << std::endl;


}
Y_UTEST_DONE()

