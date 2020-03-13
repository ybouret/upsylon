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
    void make_for(const size_t         cores,
                  const Layout<COORD> &full,
                  const COORD          preferred)
    {
        std::cerr << "|_In " << full.Dimensions << "D for " << full << std::endl;
        typename Layout<COORD>::Loop pbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());
        for(pbc.boot(); pbc.good(); pbc.next())
        {
            std::cerr << " |_boundaries=" << pbc.value << std::endl;
            std::cerr << " |_preferred =" << preferred << ": ";
            Split<COORD> ctx(cores,full,pbc.value,preferred);
            std::cerr << "optimal=" << ctx.optimal << " favored=" << ctx.favored << "(for " << ctx.mappings << ")" << std::endl;
        }


    }

}

Y_UTEST(oxide_par)
{
    const Coord3D  lower(1,1,1);
    Coord3D        upper(16,16,16);
    Coord3D        pref(1,1,1);

    if(argc>1)
    {
        upper = Coord::Parse<Coord3D>(argv[1],"upper");
    }

    if(argc>2)
    {
        pref = Coord::Parse<Coord3D>(argv[2],"pref");
    }

    for(size_t cores=1;cores<=8;++cores)
    {
        std::cerr << "<cores=" << cores << ">" << std::endl;

        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        make_for(cores,full1D,pref.x);
        make_for(cores,full2D,pref.xy());
        make_for(cores,full3D,pref);

        std::cerr << "<cores=" << cores << "/>" << std::endl << std::endl;
    }
}
Y_UTEST_DONE()

