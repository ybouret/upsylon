
#include "y/ink/engine.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    //! just for a callable prototype
    struct dummyOps
    {
        void operator()(const Area &tile, lockable &sync )
        {
            Y_LOCK(sync);
            std::cerr << "dummyOps@tile=" << tile << std::endl;
        }
    };
}

Y_UTEST(areas)
{

    Dispatcher __par  = new concurrent::simd();
    Dispatcher __seq  = new concurrent::sequential_for();

    std::cerr << "sizeof(Area)=" << sizeof(Area) << std::endl;
    
    for(size_t n=1;n<=32;++n)
    {
        coord s = coord::sqrt_of(n);
        std::cerr << n << "->" << s << std::endl;
    }

    if(argc>3)
    {
        const unit_t w   = string_convert::to<unit_t>(argv[1],"w");
        const unit_t h   = string_convert::to<unit_t>(argv[2],"h");
        const size_t n   = string_convert::to<size_t>(argv[3],"n");
        const Area   A0  = Area( coord(0,0), w,h );
        std::cerr << "Manual Tiles" << std::endl;
        Tiles zones(A0,n);
        std::cerr << std::endl;

        dummyOps ops;
        std::cerr << "Sequential Tiles" << std::endl;
        Engine seq(__seq,A0);
        seq.run(ops);
        std::cerr << std::endl;

        std::cerr << "Parallel Tiles" << std::endl;
        Engine par(__par,A0);
        par.run(ops);
        std::cerr << std::endl;

    }
    else
    {
        for(int iter=1;iter<=2;++iter)
        {
            Area A0( coord(1+alea.leq(100),1+alea.leq(100)), coord(1+alea.leq(100),1+alea.leq(100)) );
            for(size_t n=2;n<=10;++n)
            {
                Tiles zones(A0,n);
            }
        }
    }

}
Y_UTEST_DONE()
