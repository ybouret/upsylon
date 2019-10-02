#include "y/oxide/grid/structured.hpp"
#include "y/oxide/grid/rectilinear.hpp"
#include "y/oxide/grid/curvilinear.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Oxide;

namespace {


    static inline float makeValue( const float x ) throw()
    {
        return x*x;
    }

    static inline float makeValue( const point2d<float> v ) throw()
    {
        return v.norm2();
    }

    static inline float makeValue( const point3d<float> v ) throw()
    {
        return v.norm2();
    }



    template <typename COORD> static inline
    void make_grids( const COORD lower, const COORD upper )
    {
        vtk &VTK = vtk::instance();

        const Layout<COORD> L(lower,upper);
        typedef typename Grid<COORD,float>::vertex vtxf;

        std::cerr << "-- In " << L.Dimensions << "D" << " --" << std::endl;
        std::cerr << "\tlayout=" << L << std::endl;
        std::cerr << std::endl;

        vtxf org = Grid<COORD,float>::Zero();
        vtxf spc = Grid<COORD,float>::Ones();
        

        StructuredGrid<COORD,float>  sgf(L,org,spc/2);
        RectilinearGrid<COORD,float> rgf(L);
        CurvilinearGrid<COORD,float> cgf(L);


        rgf.mapRegular(L,org,spc);
        cgf.mapRegular(L,org,spc);

        typedef typename FieldFor<COORD,float>::Type fField;

        fField sgF( "f", sgf );
        fField rgF( "f", rgf );
        fField cgF( "f", cgf );

        if(true)
        {
            typename Layout<COORD>::Loop loop(L.lower,L.upper);
            for(loop.start(); loop.valid(); loop.next())
            {
                sgF(loop.value) = makeValue(sgf( loop.value ));
                rgF(loop.value) = makeValue(rgf( loop.value ));
                cgF(loop.value) = makeValue(cgf( loop.value ));
            }

        }
        
        {
            const string  fn = vformat("sg%ud.vtk", unsigned( sgf.Dimensions ));
            ios::ocstream fp(fn);
            VTK.writeHeader(fp);
            VTK.writeTitle(fp, "structured grid");
            sgf.write(VTK, fp, sgf);
            VTK.writePointData(fp,sgF);
            VTK.writeField(fp, sgF, sgF);
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




