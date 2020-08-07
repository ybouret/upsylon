#include "y/graphic/ops/filter/gradients.hpp"

namespace upsylon {

    namespace Graphic {

        Gradients:: ~Gradients() throw()
        {
        }

        Gradients:: Gradients(const Kernel::Filter::Pointer &GX,
                              const Kernel::Filter::Pointer &GY) throw() :
        Object(),
        X(GX),
        Y(GY)
        {
        }


        Gradients:: Gradients(const Gradients &G) throw() :
        object(),
        Object(),
        X(G.X),
        Y(G.Y)
        {
        }

        
    }
}
