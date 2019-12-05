
#include "y/graphic/color/data2rgba.hpp"

namespace upsylon {

    namespace Graphic {

        Data2RGBA:: ~Data2RGBA() throw()
        {
        }

        Data2RGBA:: Data2RGBA() throw()
        {
        }
        
        RGBA Data2RGBA:: operator()( const void *addr ) throw()
        {
            assert(addr);
            return get(addr);
        }

    }

}
