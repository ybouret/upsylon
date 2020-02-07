#include "y/string/esma.hpp"
#include "y/utest/run.hpp"
#include "y/type/int2int.hpp"

#include "support.hpp"

using namespace upsylon;

namespace {

    struct onFind
    {
        bool operator()( size_t pos )
        {
            std::cerr << "+@" << pos << std::endl;
            return true;
        }
    };
}

Y_UTEST(esma)
{

    const string            x = "llo";
    core::string<ptrdiff_t> k(x.size(),as_capacity,true);
    const string            y = "hello, hello, worllo!";

    core::esma::build( *k, *x, i2i<ptrdiff_t>(x.size()) );
    std::cerr << "x=" << x << std::endl;
    std::cerr << "k=" << k << std::endl;

    onFind proc;
    core::esma::find(*y, y.size(), *x, x.size(), *k, proc);
    

}
Y_UTEST_DONE()

