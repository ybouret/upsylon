#include "y/spade/vtk.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Spade;


template <typename T>
void doVTK()
{
    vtk &VTK = vtk::instance();
    ios::ocstream fp(ios::cstderr);

    std::cerr << type_name_of<T>() << std::endl;

    for(size_t i=1+alea.leq(10);i>0;--i)
    {
        const T tmp = support::get<T>();
        VTK(fp << '\t', tmp) << '\n';
    }

    std::cerr << std::endl;
}

Y_UTEST(vtk)
{
#if 0
    VTK::Writers &fmt = VTK::Writers::instance();

    std::cerr << "#native=" << fmt.ndb.entries() << std::endl;
    std::cerr << "#other =" << fmt.wdb.entries() << std::endl;

    std::cerr << "for int   : " << fmt.formatString<int>() << std::endl;
    std::cerr << "for float : " << fmt.formatString<float>() << std::endl;
#endif

    vtk &VTK = vtk::instance();


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

    std::cerr << "format for int:   " << VTK.getNative<int>().format   << std::endl;
    std::cerr << "format for float: " << VTK.getNative<float>().format << std::endl;


}
Y_UTEST_DONE()
