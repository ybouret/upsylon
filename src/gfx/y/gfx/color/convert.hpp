
//! \file


#ifndef Y_GRAPHIC_COLOR_CONVERT_INCLUDED
#define Y_GRAPHIC_COLOR_CONVERT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"
#include <cmath>

namespace upsylon
{
    namespace GFX
    {
        
        //______________________________________________________________________
        //
        //! basic conversion helpers
        //______________________________________________________________________
        struct Conv
        {
            static const  float   UnitFloat[256]; //!< 0/255,...255/255
            //! [0:1] -> [0:255]
            static inline uint8_t ToByte(const float f) throw()
            {
                return uint8_t( floorf(255.0f*f+0.5f) );
            }
            
            //! greyscale compression table
            static  const uint8_t Table[255*3+1];
            
            //! alpha function
            static  uint8_t       Alpha(const uint8_t x, const uint8_t alpha) throw();
        };
        
        
        //______________________________________________________________________
        //
        //
        //! built-in conversion
        //
        //______________________________________________________________________
        template <typename U, typename T>
        struct Convert
        {
            //! T->U
            static U From(const T &x) throw();
        };
        
        //______________________________________________________________________
        //
        //
        //! built-in auto-conversion
        //
        //______________________________________________________________________
        
        template <typename T>
        struct Convert<T,T>
        {
            //! T->T
            static const T & From(const T &x) throw() { return x; }
        };
        
 
        
    }
    
}

#endif
