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
        std::cerr << "\tDim=" << Coord::Get<COORD>::Dimensions << std::endl;
        COORD z = Coord::Zero<COORD>();
        COORD u = Coord::Ones<COORD>();
        COORD n = Coord::Natural( 100*u, alea);
        COORD i = Coord::Integer( 10*u,  alea);
        Coord::Disp(std::cerr << "\tz=",z) << std::endl;
        Coord::Disp(std::cerr << "\tu=",u) << std::endl;
        Coord::Disp(std::cerr << "\tn=",n) << std::endl;
        Coord::Disp(std::cerr << "\ti=",i) << std::endl;
    }
}

Y_UTEST(types)
{
    show<Coord1D>();
    show<Coord2D>();
    show<Coord3D>();

}
Y_UTEST_DONE()
