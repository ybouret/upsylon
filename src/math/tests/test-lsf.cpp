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

    Fit::Variables var1;
    var1("t0")("D",var["D1"]);
    std::cerr << "var1=" << var1 << std::endl;

    Fit::Variables var2;
    var2("t0")("D",var["D2"]);
    std::cerr << "var2=" << var2 << std::endl;

    const size_t   nvar = var.size();
    vector<double> aorg(nvar);

    var(aorg,"t0") = 1.0;

    var1(aorg,"D") = 1e-8;

    var2(aorg,"D") = 1e-6;

    std::cerr << "aorg=" << aorg << std::endl;



}
Y_UTEST_DONE()


