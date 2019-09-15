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
        vtk          &VTK = vtk::instance();
        ios::ocstream fp( ios::cstderr );
        for(size_t iter=0;iter<8;++iter)
        {
            const T tmp = support::get<T>();
            VTK.write(fp << "\t<",tmp) << '>' << '\n';
        }
        std::cerr << "<" << typeid(T).name() << "/>" << std::endl;
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
    
}
Y_UTEST_DONE()


