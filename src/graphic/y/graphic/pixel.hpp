
//! \file

#ifndef Y_GRAPHIC_PIXEL_INCLUDED
#define Y_GRAPHIC_PIXEL_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        struct Pixel
        {

            
            template <typename T>
            static void Zero( T & ) throw();

            template <typename T>
            static bool IsZero( const T & ) throw();


            template <typename T>
            static T Invert( const T & ) throw();

        };
    }
}

#endif

