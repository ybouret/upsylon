//! \file

#ifndef Y_COLOR_CONVERT_INCLUDED
#define Y_COLOR_CONVERT_INCLUDED 1

#include "y/color/rgb.hpp"
#include "y/color/yuv.hpp"
#include <cmath>

namespace upsylon
{

    namespace core
    {
        //______________________________________________________________________
        //
        //! basic conversion helpers
        //______________________________________________________________________
        struct convert_color
        {
            static const  float   unit_float[256]; //!< 0/255,...255/255

            //! [0:1] -> [0:255]
            static inline uint8_t to_byte(const float f) throw()
            {
                return uint8_t( floorf(255.0f*f+0.5f) );
            }
            
            //! greyscale compression table
            static  const uint8_t greyscale[255*3+1];

            //! fg:alpha, bg:beta, beta=255-alpha
            static uint8_t mix(const uint8_t fg,
                               const uint8_t bg,
                               const uint8_t alpha,
                               const uint8_t beta) throw();

        };
    }

    //______________________________________________________________________
    //
    //
    //! built-in conversion
    //
    //______________________________________________________________________
    template <typename U, typename T>
    struct convert_color
    {
        //! T->U
        static U from(const T &x) throw();
    };

    //______________________________________________________________________
    //
    //
    //! built-in auto-conversion
    //
    //______________________________________________________________________

    template <typename T>
    struct convert_color<T,T>
    {
        //! T->T
        static const T & from(const T &x) throw() { return x; }
    };




}

#endif
