

#include "y/gfx/color/rgb.hpp"

namespace upsylon {

    namespace graphic
    {
        namespace crux {

            template <> const uint8_t rgba<uint8_t>::Opaque = 0xff;
            template <> const float   rgba<float>  ::Opaque = 1.0f;
            template <> const double  rgba<double> ::Opaque = 1.0;

#include "saturated.inc"
            
        }
    }
    
}
