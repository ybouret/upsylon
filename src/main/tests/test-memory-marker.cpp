#include "y/memory/marker.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(marker)
{
    memory::marker m0;        std::cerr << "m0=" << m0  << std::endl;
    memory::marker m1(1,13);  std::cerr << "m1=" << m1  << std::endl;
    memory::marker m2(m1,7);  std::cerr << "m2=" << m2  << std::endl;
    memory::marker m3(m2,31); std::cerr << "m3=" << m3  << std::endl;

}
Y_UTEST_DONE()

