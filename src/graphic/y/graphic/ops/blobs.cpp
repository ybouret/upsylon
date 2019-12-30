
#include "y/graphic/ops/blobs.hpp"

namespace upsylon {

    namespace Graphic {

        const Point Blobs::delta[8] =
        {
            Point(-1,0),
            Point(1,0),
            Point(0,-1),
            Point(0,1),

            Point(1,1),
            Point(1,-1),
            Point(-1,-1),
            Point(-1,1)

        };
        
    }

}
