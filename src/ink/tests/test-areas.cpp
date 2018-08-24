
#include "y/ink/engine.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace ink;

namespace
{
    //! just for a callable prototype
    struct dummyOps
    {
        void operator()(const area &zone, lockable &sync )
        {
            Y_LOCK(sync);
            std::cerr << "dummyOps@zone=" << zone << std::endl;
        }
    };
}

Y_UTEST(areas)
{

    dispatcher __par  = new concurrent::simd();
    dispatcher __seq  = new concurrent::sequential_for();

    std::cerr << "sizeof(area)=" << sizeof(area) << std::endl;
    
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
        const area   A0  = area( coord(0,0), w,h );
        std::cerr << "Manual Tiles" << std::endl;
        areas zones(A0,n);
        std::cerr << std::endl;

        dummyOps ops;
        std::cerr << "Sequential Tiles" << std::endl;
        engine seq(__seq,A0);
        seq.run(ops);
        std::cerr << std::endl;

        std::cerr << "Parallel Tiles" << std::endl;
        engine par(__par,A0);
        par.run(ops);
        std::cerr << std::endl;

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
