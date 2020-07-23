#include "y/counting/permutations.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace {

    void display( const accessible<size_t> &conf )
    {
        std::cerr <<  '[';
        for(size_t i=1;i<=conf.size();++i)
        {
            const size_t value = conf[i];
            if(value<=0)
            {
                std::cerr << '|';
            }
            else
            {
                std::cerr << char('a'+(value-1));
            }
        }
        std::cerr << ']' << std::endl;
    }
}

Y_UTEST(drawers)
{
    if(argc>1)
    {
        vector<size_t> data;
        const size_t   drawers = string_convert::to<size_t>(argv[1],"drawers");
        Y_CHECK(drawers>0);
        for(size_t i=drawers;i>1;--i) data << 0;

        for(int i=2;i<argc;++i)
        {
            const size_t socks = string_convert::to<size_t>(argv[i],"socks");
            for(size_t j=socks;j>0;--j) data << (i-1);
        }
        std::cerr << "data=" << data << std::endl;


        permutations<size_t> perm(data);
        std::cerr << "#possibilities: " << perm.count << std::endl;
        for( perm.boot(); perm.good(); perm.next() )
        {
            display(perm);
        }
    }
}
Y_UTEST_DONE()

