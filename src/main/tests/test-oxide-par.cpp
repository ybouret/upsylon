#include "y/oxide/field/split.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Oxide;

namespace {

    template <typename COORD> static inline
    void test_par( const size_t cores, const Layout<COORD> &full, const COORD pbc)
    {
        typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );

        for(loop.start();loop.valid();loop.next())
        {
            const COORD &preferred = loop.value;
            std::cerr << "  |_preferred=" << preferred << std::endl;
            Split<COORD> ctx(cores,full,pbc,preferred);
            std::cerr << "   |_ optimal=" << ctx.optimal << ", favorite=" << ctx.favorite << std::endl;
        }
    }


    template <typename COORD> static inline
    void make_for(const size_t cores, const Layout<COORD> &full )
    {
        std::cerr << "|_In " << full.Dimensions << "D for " << full << std::endl;
        typename Layout<COORD>::Loop pbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());
        for(pbc.start(); pbc.valid(); pbc.next())
        {
            std::cerr << " |_boundaries=" << pbc.value << std::endl;
            test_par<COORD>(cores,full,pbc.value);
        }


    }

}

Y_UTEST(oxide_par)
{
    const Coord3D  lower(1,1,1);
    Coord3D        upper(16,16,16);
    if(argc>1)
    {
        upper = Coord::Parse<Coord3D>(argv[1],"upper");
    }

    for(size_t cores=1;cores<=8;++cores)
    {
        std::cerr << "<cores=" << cores << ">" << std::endl;

        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        make_for(cores,full1D);
        make_for(cores,full2D);
        //make_for(cores,full3D);

        std::cerr << "<cores=" << cores << "/>" << std::endl << std::endl;
    }
}
Y_UTEST_DONE()

