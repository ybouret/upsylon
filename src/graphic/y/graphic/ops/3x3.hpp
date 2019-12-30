
//! \file
#ifndef Y_GRAPHIC3x3_INCLUDED
#define Y_GRAPHIC3x3_INCLUDED

#include "y/graphic/pixel.hpp"
#include "y/graphic/pixmaps.hpp"

namespace upsylon {

    namespace Graphic {

        struct Apply3x3
        {

            template <typename T,
            typename PROC>
            void On( Pixmap<T>      &target,
                    const Pixmap<T> &source,
                    PROC            &proc,
                    const Tile      &tile )
            {
                
            }


        };

    }
}

#endif

