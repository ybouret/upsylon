#include "y/string/esma.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

Y_UTEST(esma)
{

    const string            x = "llo";
    core::string<ptrdiff_t> k(x.size(),as_capacity,true);
    const string            y = "hello, hello, worllo!";

    core::esma::build( *k, *x, x.size() );
    std::cerr << "x=" << x << std::endl;
    std::cerr << "k=" << k << std::endl;

    core::esma::find(*y, y.size(), *x, x.size(), *k);
    

}
Y_UTEST_DONE()

