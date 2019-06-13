#include "y/math/kernel/cyclic.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(cyclic)
{
    cyclic<double> C(20,1);

    std::cerr << "#count              = " << C.count              << std::endl;
    std::cerr << "#tridiag_extraneous = " << C.tridiag_extraneous << std::endl;
    std::cerr << "#cyclic_offset      = " << C.cyclic_offset      << std::endl;
    std::cerr << "#cyclic_extraneous  = " << C.cyclic_extraneous  << std::endl;


}
Y_UTEST_DONE()


