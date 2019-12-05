
#include "y/graphic/color/rgba2data.hpp"

namespace upsylon {

    namespace Graphic {

        RGBA2Data:: ~RGBA2Data() throw()
        {
        }

        RGBA2Data:: RGBA2Data() throw()
        {
        }

        void RGBA2Data:: operator()( void *data, const RGBA &C ) throw()
        {
            assert(data);
            put(data,C);
        }
        
    }

}


