
#include "y/type/ints-to-real.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/type/spec.hpp"
#include <cfloat>

using namespace upsylon;

template <typename REAL>
bool test_i2r( const uint64_t u );

template <>
inline bool test_i2r<float>( const uint64_t u )
{
    const float    f = float(u);
    const uint64_t n = uint64_t(f);
    if(u!=n)
    {
        std::cerr << u << "->" << f << "->" << n << std::endl;
    }
    return u==n;
}

template <>
inline bool test_i2r<double>( const uint64_t u )
{
    const double    f = double(u);
    const uint64_t  n = uint64_t(f);
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

#include <iomanip>
static inline void i_display(int i)
{
    std::cerr << std::dec;
    std::cerr << std::setw(4) << i;
    std::cerr << std::hex;
    {
        union {
            float    f;
            uint32_t j;
        } alias = { static_cast<float>(i) };

        std::cerr << " --> " << alias.f << "\t0x" << std::setw(8) << alias.j;
    }
    {
        union {
            double   f;
            uint64_t j;
        } alias = { static_cast<double>(i) };

        std::cerr << " --> " << alias.f << "\t0x" << std::setw(16) << alias.j;
    }
    std::cerr << std::dec;
    std::cerr << std::endl;
}

template <typename T, typename I> static inline
bool check_i2r( const I value ) throw()
{
    const T r = static_cast<T>(value);
    const I i = static_cast<I>(r);
    return value == i;
}

template <typename T> static inline void display_i2r()
{
    typedef typename  i2r<T>::itype itype;

    std::cerr << "i2r<" << type_name_of<T>() << ">" << std::endl;
    std::cerr << "|_integer : <" << type_name_of<itype>() << ">" << std::endl;
    std::cerr << "|_mantissa: " <<  i2r<T>::info_type::mantissa << std::endl;
    std::cerr << "|_minimum : " <<  i2r<T>::minimum  << std::endl;
    std::cerr << "|_maximum : " <<  i2r<T>::maximum  << std::endl;

    Y_CHECK( (check_i2r<T,itype>(i2r<T>::minimum)) );
    Y_CHECK( (check_i2r<T,itype>(i2r<T>::minimum)) );

    std::cerr << "CHECK inside..." << std::endl;
    for(size_t iter=0;iter<100000;++iter)
    {
        const itype tmp = alea.narrow(i2r<T>::minimum,i2r<T>::maximum);
        Y_ASSERT( (check_i2r<T,itype>(tmp)) );
    }

    for(int i=-65536; i<= 65536; ++i)
    {
        const T tmp = i2r<T>::_(i);
        Y_ASSERT( static_cast<int32_t>(tmp) == i );
    }

    std::cerr << std::endl;
}

static float        ftable[384] = { 0 };
static const float *fentry      = &ftable[128];


static inline void doCast()
{
    for(int i=-128;i<=255;++i)
    {
        volatile float f = static_cast<float>(i);
        (void)f;
    }
}

static inline void doCall()
{
    for(int i=-128;i<=255;++i)
    {
        volatile float f = fentry[i];
        (void)f;
    }
}

#include "y/ios/ocstream.hpp"

Y_UTEST(i2r)
{
    std::cerr << "Radix:" << std::endl;
    std::cerr << '\t' << FLT_RADIX << std::endl;
    std::cerr << std::endl;

    std::cerr << "Mantissa:" << std::endl;
    std::cerr << '\t' << FLT_MANT_DIG  << std::endl;
    std::cerr << '\t' << DBL_MANT_DIG  << std::endl;
    std::cerr << '\t' << LDBL_MANT_DIG << std::endl;
    std::cerr << std::endl;
    
    const uint64_t uf = find_i2r<float>(0);
    std::cerr << "i2r<float>  = " << uf << ", bits=" <<   bits_for(uf) << std::endl;
    {
        const int64_t  x = -int64_t(uf);
        std::cerr << "\tsigned : " << x << std::endl;
        const float    m  = static_cast<float>(x);
        std::cerr << "\tminus  : " << m << std::endl;
        const int64_t  r = static_cast<int64_t>(m);
        std::cerr << "\tback   : " << r << std::endl;
    }

    const uint64_t ud = find_i2r<double>( (uint64_t(1) << 53) - 10000000 );
    std::cerr << "i2r<double> = " << ud << ", bits=" << bits_for(ud) << std::endl;
    {
        const int64_t  x = -int64_t(ud);
        std::cerr << "\tsigned : " << x << std::endl;
        const float    m  = static_cast<float>(x);
        std::cerr << "\tminus  : " << m << std::endl;
        const int64_t  r = static_cast<int64_t>(m);
        std::cerr << "\tback   : " << r << std::endl;
    }


    for(int i=-5;i<=5;++i)
    {
        i_display(i);
    }

    std::cerr << std::endl;
    display_i2r<float>();
    display_i2r<double>();

    std::cerr << "int8_t::min   = " << int(signed_int<1>::minimum)   << std::endl;
    std::cerr << "int8_t::max   = " << int(signed_int<1>::maximum)   << std::endl;

    std::cerr << "uint8_t::min  = " << int(unsigned_int<1>::minimum) << std::endl;
    std::cerr << "uint8_t::max  = " << int(unsigned_int<1>::maximum) << std::endl;

    std::cerr << "int16_t::min  = " << int(signed_int<2>::minimum)   << std::endl;
    std::cerr << "int16_t::max  = " << int(signed_int<2>::maximum)   << std::endl;

    std::cerr << "uint16_t::min = " << int(unsigned_int<2>::minimum) << std::endl;
    std::cerr << "uint16_t::max = " << int(unsigned_int<2>::maximum) << std::endl;

    if(false)
    {
        ios::ocstream fp("byte2float.dat");
        int count = 0;
        for(int i=-128;i<=255;++i)
        {
            fp("%d.0f",i);
            if(i<255) fp << ',';
            if( 0 == (i%8) ) fp << '\n';
            ++count;
        }
        std::cerr << "count=" << count << std::endl;
    }

    {
        float *p = ftable;
        for(int i=-128;i<=255;++i)
        {
            *(p++) = float(i);
        }
        Y_CHECK( fabsf(fentry[0]) <= 0);
        for(int i=-128;i<=255;++i)
        {
            Y_ASSERT( int( fentry[i] ) == i );
        }
    }

    std::cerr << "casting..." << std::endl;
    const double duration   = 2;
    double       cast_speed = 0;
    Y_TIMINGS(cast_speed,duration,doCast());
    std::cerr << "cast_speed=" << cast_speed << std::endl;

    double call_speed = 0;
    Y_TIMINGS(call_speed,duration,doCall());
    std::cerr << "call_speed=" << call_speed << std::endl;




}
Y_UTEST_DONE()

