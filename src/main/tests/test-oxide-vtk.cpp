#include "y/oxide/vtk.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Oxide;


Y_UTEST(oxide_vtk)
{
    vtk &VTK = vtk::instance();
    
    std::cerr << "VTK.life_time=" << VTK.life_time << std::endl;
    
}
Y_UTEST_DONE()


