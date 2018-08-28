#include "y/math/fit/variables.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(lsf)
{
    Fit::Variables var;
    var.create_global("A");
    std::cerr << "var=" << var << std::endl;
}
Y_UTEST_DONE()


