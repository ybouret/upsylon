
//! \file


#ifndef Y_GRAPHIC_COLOR_CONVERT_INCLUDED
#define Y_GRAPHIC_COLOR_CONVERT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"


namespace upsylon
{
    namespace GFX
    {

        template <typename T>
        class Convert
        {
        public:            
            template <typename U> static
            T From(const U &) throw();
            
            template <> static
            T From(const T &x) throw() { return x; }
        };
        
        
        class ConvertMono : public Convert<uint8_t>
        {
        public:
            static const   float UnitFloat[256]; //!< precomputed levels
            static uint8_t Compress(const uint8_t r, const uint8_t g, const uint8_t b) throw();
        };
        

        
    }

}

#endif
