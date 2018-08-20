
#include "y/ink/bitmap.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace ink;

Y_UTEST(areas)
{

    std::cerr << "sizeof(area)=" << sizeof(area) << std::endl;
    if(argc>3)
    {

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
