#include "y/oxide/grid/structured.hpp"
#include "y/oxide/grid/rectilinear.hpp"
#include "y/oxide/grid/curvilinear.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Oxide;

namespace {

    template <typename COORD>
    void make_grids( const COORD lower, const COORD upper )
    {
        const Layout<COORD> L(lower,upper);
        typedef typename Grid<COORD,float>::vertex vtxf;

        std::cerr << "-- In " << L.Dimensions << "D" << " --" << std::endl;
        std::cerr << "\tlayout=" << L << std::endl;
        std::cerr << std::endl;

        vtxf org = Grid<COORD,float>::Zero();
        vtxf spc = Grid<COORD,float>::Ones();
        

        StructuredGrid<COORD,float>  sgf(L,org,spc);
        RectilinearGrid<COORD,float> rgf(L);
        CurvilinearGrid<COORD,float> cgf(L);


        rgf.mapRegular(L,org,spc);
        cgf.mapRegular(L,org,spc);

        typename Layout<COORD>::Loop loop(L.lower,L.upper);
        for(loop.start(); loop.valid(); loop.next())
        {
            (void)sgf( loop.value );
            (void)rgf( loop.value );
            (void)cgf( loop.value );
        }

    }


}

Y_UTEST(oxide_grids)
{
    const Coord3D  lower(1,1,1);
    Coord3D        upper(16,16,16);

    if(argc>1)
    {
        upper = Coord::Parse<Coord3D>(argv[1],"upper");
    }

    make_grids(lower.x,upper.x);
    make_grids(lower.xy(),upper.xy());
    make_grids(lower,upper);


}
Y_UTEST_DONE()




