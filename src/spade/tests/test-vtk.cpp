#include "y/spade/vtk/formats.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Spade;


template <typename T>
void doVTK()
{
    VTK::Formats &fmt = VTK::Formats::instance();
    ios::ocstream fp(ios::cstderr);

    std::cerr << type_name_of<T>() << std::endl;

    for(size_t i=1+alea.leq(10);i>0;--i)
    {
        const T tmp = support::get<T>();
        fmt.write(fp << '\t', tmp) << '\n';
    }

    std::cerr << std::endl;
}

Y_UTEST(vtk)
{
    VTK::Formats &fmt = VTK::Formats::instance();

    std::cerr << "#primary=" << fmt.pdb.entries() << std::endl;
    std::cerr << "#formats=" << fmt.fdb.entries() << std::endl;

    std::cerr << "for int   : " << fmt.formatString<int>() << std::endl;
    std::cerr << "for float : " << fmt.formatString<float>() << std::endl;

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

}
Y_UTEST_DONE()
