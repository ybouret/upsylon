#include "y/oxide/vtk.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Oxide;


namespace  {
    
}

Y_UTEST(oxide_vtk)
{
    vtk &VTK = vtk::instance();
    
    std::cerr << "VTK.life_time=" << VTK.life_time << std::endl;
    
    
    {
        ios::ocstream fp( ios::cstderr );
        
        VTK.write<int>(fp,3)      << '\n';
        VTK.write<float>(fp,1.2f) << '\n';
        VTK.write<double>(fp,1.4) << '\n';
        VTK.write<point2d<double> >(fp,point2d<double>(1,2)) << '\n';
        VTK.write<complex<float> >(fp,complex<float>(3,4)) << '\n';
        VTK.write< point3d<double> >(fp,point3d<double>(5,6,7)) << '\n';

        
        
    }
    
    
}
Y_UTEST_DONE()


