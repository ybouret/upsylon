#include "y/math/fit/variables.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(lsf)
{
    Fit::Variables var;
    var << "t0" << "D1" << "D2";
    std::cerr << "var=" << var << std::endl;

    {
        Fit::Variables tmp = var;
        std::cerr << "tmp=" << tmp << std::endl;
        tmp.release();
        tmp = var;
        std::cerr << "tmp=" << tmp << std::endl;
    }
}
Y_UTEST_DONE()


