
//! \file


#ifndef Y_GRAPHIC_COLOR_CONVERT_INCLUDED
#define Y_GRAPHIC_COLOR_CONVERT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"


namespace upsylon
{
    namespace GFX
    {
        
        namespace Kernel
        {
            struct Conv
            {
                static const  float   UnitFloat[256];
                static inline uint8_t ToByte(const float f) throw()
                {
                    return uint8_t( floorf(255.0f*f+0.5f) );
                }
                static  const uint8_t Table[255*3+1];
                static  uint8_t       Alpha(const uint8_t x, const uint8_t alpha) throw();
                
            };
        };
        
        template <typename T>
        class Convert
        {
        public:            
            template <typename U> static
            U To(const T &) throw();
            
            template <> static
            T To(const T &x) throw() { return x; }
        };
        
        
        
    }

}

#endif
