#include "y/string/esma.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"


#include "support.hpp"

using namespace upsylon;

namespace {

    struct onFind
    {
        const char *content;
        bool operator()( size_t pos )
        {
            std::cerr << "+@" << pos;
            if(content) std::cerr << " in '" << content << "'";
            std::cerr << std::endl;
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

    onFind proc = { 0 };

    if(true)
    {
        const string            x = "llo";
        core::string<unit_t>    k(x.size()+1,as_capacity,true);
        const string            y = "hello, hello, worllo!";

        core::esma::build( *k, *x, x.ssize() );
        std::cerr << "x=" << x << std::endl;
        std::cerr << "k=" << k << std::endl;

        core::esma::find(*y, y.ssize(), *x, x.ssize(), *k, proc);
    }

    if(argc>1)
    {
        const string         x = argv[1];
        core::string<unit_t> k( x.size()+1, as_capacity, true);
        core::esma::build( *k, *x, x.ssize() );
        std::cerr << "k['" << x << "']=" << k << std::endl;
        if(argc>2)
        {
            const string  fileName = argv[2];
            ios::icstream fp(fileName);
            string line;
            while( fp.gets(line) )
            {
                proc.content = *line;
                core::esma::find(*line, line.ssize(), *x, x.ssize(), *k, proc);
            }
        }
    }


}
Y_UTEST_DONE()

