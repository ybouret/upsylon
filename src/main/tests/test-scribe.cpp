#include "y/ios/scribes.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

#define Y_CHECK_BASIC(TYPE)      \
Y_CHECK( io.has_basic<TYPE>() ); \
Y_CHECK( io.has<TYPE>()       ); \
std::cerr << "|_format: " << io.format<TYPE>() << std::endl

namespace {

    template <typename T> static inline void doTest(const ios::scribes &io)
    {
        const ios::scribe    &s = io.get<T>();
        ios::ocstream         fp( ios::cstderr );
        for(size_t i=0;i<10;++i)
        {
            const T tmp = support::get<T>();
            std::cerr << tmp << " | ";
            std::cerr.flush();
            s.put(fp,tmp) << '\n';
            fp.flush();
        }
    }
}


Y_UTEST(scribe)
{
    ios::scribes &io =  ios::scribes::instance();

    Y_CHECK_BASIC(int8_t);
    Y_CHECK_BASIC(int16_t);
    Y_CHECK_BASIC(int32_t);
    Y_CHECK_BASIC(uint8_t);
    Y_CHECK_BASIC(uint16_t);
    Y_CHECK_BASIC(uint32_t);
    Y_CHECK_BASIC(char);
    Y_CHECK_BASIC(unsigned char);
    Y_CHECK_BASIC(short);
    Y_CHECK_BASIC(unsigned short);
    Y_CHECK_BASIC(int);
    Y_CHECK_BASIC(unsigned);
    Y_CHECK_BASIC(long);
    Y_CHECK_BASIC(unsigned long);
    Y_CHECK_BASIC(float);
    Y_CHECK_BASIC(double);
    Y_CHECK(io.has<string>());

    doTest<int16_t>(io);
    doTest<long>(io);
    doTest<string>(io);
    doTest<float>(io);
    doTest<double>(io);
    doTest< complex<float>  >(io);
    doTest< complex<double> >(io);

    doTest< point2d<double> >(io);
    doTest< point2d<float> >(io);

    doTest< point3d<double> >(io);
    doTest< point3d<float> >(io);

}
Y_UTEST_DONE()

