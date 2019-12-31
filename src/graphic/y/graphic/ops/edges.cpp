
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {

        Edges:: ~Edges() throw()
        {
        }

        Edges:: Edges( const size_t W, const size_t H) :
        Pixmap<size_t>(W,H),
        gmax(0),
        g(W,H),
        G(W,H),
        L(W,H),
        hist(),
        hardThreshold(0),
        softThreshold(0)
        {
        }



    }
}
