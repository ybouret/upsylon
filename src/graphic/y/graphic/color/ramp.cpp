
#include "y/graphic/color/ramp.hpp"

namespace upsylon {

    namespace Graphic
    {
        Ramp:: ~Ramp() throw()
        {
        }

        Ramp:: Ramp() throw()  
        {

        }

        rgb Ramp:: compute(const float v, const float vmin, const float vmax)
        {
            const rgb *tab = _256colors();
            if(v<=vmin)
            {
                return tab[0];
            }
            else if( v>=vmax )
            {
                return tab[0xff];
            }
            else
            {
                return tab[ uint8_t(floorf( 255.0f * (v-vmin)/(vmax-vmin) + 0.5f )) ];
            }

        }
        
    }

}
