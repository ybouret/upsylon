
#include "y/graphic/area.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        Area:: ~Area() throw()
        {
        }

        Area:: Area( const size_t W, const size_t H) :
        w(W),
        h(H),
        items(w*h)
        {
            if( (w>0&&h==0) || (h>0&&w==0) )
            {
                throw exception("Invalid Graphic:Area %ux%u", unsigned(w), unsigned(h) );
            }
        }

    }
}

