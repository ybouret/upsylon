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
    doVTK<mpn>();
    doVTK<mpz>();
    doVTK<mpq>();

    std::cerr << "format for int:   " << VTK.getNative<int>().format   << std::endl;
    std::cerr << "format for float: " << VTK.getNative<float>().format << std::endl;

    std::cerr << "tl_index float:  " << tl::index_of<tl::std_reals_list,float>::value  << std::endl;
    std::cerr << "tl_index double: " << tl::index_of<tl::std_reals_list,double>::value << std::endl;


}
Y_UTEST_DONE()
