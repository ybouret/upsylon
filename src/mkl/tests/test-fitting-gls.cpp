#include "y/mkl/fitting/gls.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_gls)
{
    gls ls;
    gls::shared s = gls::sample::create("trial",1024);


}
Y_UTEST_DONE()

