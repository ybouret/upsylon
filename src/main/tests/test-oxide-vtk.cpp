#include "y/oxide/vtk.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;


namespace  {

    template <typename T> static inline
    void doVTK()
    {
        std::cerr << "<" << typeid(T).name() << ">" << std::endl;
        const vtk          &VTK = vtk::instance();
        ios::ocstream fp( ios::cstderr );
        for(size_t iter=0;iter<8;++iter)
        {
            const T tmp = support::get<T>();
            VTK(fp << "\t<",tmp) << '>' << '\n';
        }
        std::cerr << "<" << typeid(T).name() << "/>" << std::endl;
    }

    template <typename COORD> static inline
    void saveLayout()
    {
        const Layout<COORD> L( Coord::Integer<COORD>(10,alea), Coord::Integer<COORD>(10,alea) );
        const string filename = vformat("layout%ud.vtk", unsigned(L.Dimensions) );
        ios::ocstream fp(filename);

        const vtk &VTK = vtk::instance();
        VTK.writeHeader(fp);
        VTK.writeTitle(fp,filename);
        VTK.writeLayout(fp,L);
        

    }

}

Y_UTEST(oxide_vtk)
{
    
    doVTK<float>();
    doVTK<double>();
    doVTK<short>();
    doVTK<uint16_t>();
    doVTK< complex<float>  >();
    doVTK< complex<double> >();
    doVTK< point2d<float>  >();
    doVTK< point2d<double>  >();
    doVTK< point3d<float>  >();
    doVTK< point3d<double>  >();

    saveLayout<Coord1D>();
    saveLayout<Coord2D>();
    saveLayout<Coord3D>();




}
Y_UTEST_DONE()


