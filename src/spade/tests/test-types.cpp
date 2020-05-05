#include "y/spade/types.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline void show()
    {
        std::cerr << type_name_of<COORD>() << std::endl;
    }
}

Y_UTEST(types)
{
    show<Coord1D>();
    show<Coord2D>();
    show<Coord3D>();

}
Y_UTEST_DONE()
