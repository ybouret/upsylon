#include "y/spade/vtk.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename T>
    void doVTK()
    {
        vtk &VTK = vtk::instance();
        ios::ocstream fp(ios::cstderr);

        std::cerr << type_name_of<T>() << std::endl;
        const vtk::Writer &w = VTK.getWriter<T>();
        std::cerr << "|_type: '" << w.dataType() << "'" << std::endl;
        std::cerr << "|_comp: " << w.components() << std::endl;

        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            const T tmp = support::get<T>();
            VTK(fp << '\t', tmp) << '\n';
        }

        std::cerr << std::endl;
    }

    template <typename COORD> static inline
    void doSave()
    {
        static const Coord1D r[] = { 10, 20 , 30 };

        vtk &VTK = vtk::instance();

        const unsigned dims = Coord::Get<COORD>::Dimensions;
        const string   fn   = vformat("in%ud.vtk",dims);
        ios::ocstream  fp(fn);
        VTK.writeHeader(fp);
        VTK.writeTitle(fp,fn);

        const Layout<COORD> L( Coord::Ones<COORD>(), *(COORD *) &r[0] );
        std::cerr << "Layout: " << L << std::endl;
        VTK.writeLayout(fp,L);
    }


}





Y_UTEST(vtk)
{
    
    vtk &VTK = vtk::instance();

    std::cerr << "-- Testing Writers" << std::endl;

    doVTK<short>();
    doVTK<unsigned short>();
    doVTK<float>();
    doVTK<double>();
    doVTK< complex<float> >();
    doVTK< complex<double> >();

    doVTK< point2d<float> >();
    doVTK< point2d<double> >();

    doVTK< point3d<float> >();
    doVTK< point3d<double> >();
    doVTK<mpn>();
    doVTK<mpz>();
    doVTK<mpq>();

    std::cerr << "format for int:   " << VTK.getNative<int>().format   << std::endl;
    std::cerr << "format for float: " << VTK.getNative<float>().format << std::endl;

    std::cerr << "-- Testing VTK files" << std::endl;
    


    doSave<Coord1D>();
    doSave<Coord2D>();
    doSave<Coord3D>();

    

}
Y_UTEST_DONE()
