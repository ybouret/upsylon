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

    for(size_t length=1;length<1024;++length)
    {
        string               x(length,as_capacity,true);
        core::string<unit_t> k(length,as_capacity,true);

        assert(x.size()==length);
        assert(k.size()==length);

        for(size_t i=0;i<length;++i)
        {
            x[i] = alea.range<char>(32,126);
            k[i] = 0;
        }
        core::esma::build( *k, *x, length );

    }

    if(argc>1)
    {
        
    }


}
Y_UTEST_DONE()

