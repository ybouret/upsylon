
#include "y/graphic/color/rgb.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            template <> const uint8_t RGBA<uint8_t>::Opaque = 0xff;
            template <> const float   RGBA<float>  ::Opaque = 1.0f;
            template <> const double  RGBA<double> ::Opaque = 1.0;
            
        }

    }
}
