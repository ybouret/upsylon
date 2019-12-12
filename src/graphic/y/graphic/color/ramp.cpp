
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
            
            rgb Ramp:: operator()(const float v, const float vmin, const float vmax)
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

#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Graphic
        {
            ColorRamp_:: ~ColorRamp_() throw()
            {
            }
            
            ColorRamp_:: ColorRamp_(const Ramp::Pointer &_ramp,
                                    const float          _vmin,
                                    const float          _vmax) throw() :
            ramp( _ramp ),
            vmin( _vmin ),
            vmax( _vmax )
            {
                setRange(_vmin,_vmax);
            }
            
            void ColorRamp_:: setRange(const float _vmin, const float _vmax) throw()
            {
                aliasing::_(vmin) = min_of(_vmin,_vmax);
                aliasing::_(vmax) = max_of(_vmin,_vmax);
            }
        }
    
}

