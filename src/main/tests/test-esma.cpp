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

    std::cerr << "-- checking construction" << std::endl;
    for(size_t length=0;length<1024;++length)
    {
        string               x(length,as_capacity,true);
        core::string<unit_t> k(length+1,as_capacity,true);



        for(size_t i=0;i<length;++i)
        {
            x[i] = alea.range<char>(32,126);
            k[i] = 0;
        }
        core::esma::check_build( *k, *x, length );
        core::esma::build( *k, *x, length );
    }

    if(false)
    {
        const string            x = "llo";
        core::string<unit_t>    k(x.size(),as_capacity,true);
        const string            y = "hello, hello, worllo!";

        core::esma::build( *k, *x, x.ssize() );
        std::cerr << "x=" << x << std::endl;
        std::cerr << "k=" << k << std::endl;

        onFind proc;
        core::esma::find(*y, y.ssize(), *x, x.ssize(), *k, proc);
    }



}
Y_UTEST_DONE()

