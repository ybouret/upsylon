
#include "y/ink/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace ink;

Y_UTEST(areas)
{

    std::cerr << "sizeof(area)=" << sizeof(area) << std::endl;
    
    for(size_t n=1;n<=256;++n)
    {
        size_t lo = isqrt(n);
        size_t hi = n/lo;
        while(hi*lo!=n)
        {
            --lo;
            hi = n/lo;
        }
        std::cerr << n << "->" << lo  << "*" << hi << std::endl;

    }
    return 0;
    
    if(argc>3)
    {
        const unit_t w   = string_convert::to<unit_t>(argv[1],"w");
        const unit_t h   = string_convert::to<unit_t>(argv[2],"h");
        const size_t n   = string_convert::to<size_t>(argv[3],"n");
        const area   A0  = area( coord(0,0), coord(w,h), area_sizes);
        areas zones(A0,n);
    }
    else
    {
        for(int iter=1;iter<=2;++iter)
        {
            area A0( coord(1+alea.leq(100),1+alea.leq(100)), coord(1+alea.leq(100),1+alea.leq(100)) );
            for(size_t n=2;n<=10;++n)
            {
                areas zones(A0,n);
            }
        }
    }

}
Y_UTEST_DONE()
