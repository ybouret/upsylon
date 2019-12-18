#include "y/graphic/ops/filter/gradients.hpp"

namespace upsylon {

    namespace Graphic {

        Gradients:: ~Gradients() throw()
        {
        }

        Gradients:: Gradients(const Kernel::Filter::Pointer &GX,
                              const Kernel::Filter::Pointer &GY) throw() :
        X(GX),
        Y(GY)
        {
        }


        Gradients:: Gradients(const Gradients &G) throw() :
        X(G.X),
        Y(G.Y)
        {
        }

        
    }
}
