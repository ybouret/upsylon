#include "y/spade/vtk/format.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(vtk)
{
    const type_spec &short_ts = type_spec_of<short>();
    const type_spec &int16_ts = type_spec_of<int16_t>();
    std::cerr << short_ts << std::endl;
    std::cerr << int16_ts << std::endl;

}
Y_UTEST_DONE()
