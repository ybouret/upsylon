#include "y/spade/vtk/format.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Spade;

Y_UTEST(vtk)
{
#if 0
    const type_spec &short_ts = type_spec_of<short>();
    const type_spec &int16_ts = type_spec_of<int16_t>();
    std::cerr << short_ts << std::endl;
    std::cerr << int16_ts << std::endl;

    VTK::Format &fmt = VTK::Format::instance();

    fmt.use<short>("%h");
#endif

}
Y_UTEST_DONE()
