#include "y/math/fit/variables.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(lsf)
{
    Fit::Variables var;
    var << "t0" << "D1" << "D2";
    std::cerr << "var=" << var << std::endl;

    Fit::Variables var1;
    var1("t0")("D","D1");
    std::cerr << "var1=" << var1 << std::endl;

    Fit::Variables var2;
    var2("t0")("D","D2");
    std::cerr << "var2=" << var2 << std::endl;

}
Y_UTEST_DONE()


