
//! \file


#ifndef Y_GFX_INTERPOLATION_INCLUDED
#define Y_GFX_INTERPOLATION_INCLUDED 1

#include "y/gfx/pixel/blend.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! function to interpolate pixels
        struct interp
        {
            //! x[0..N-1] from 0<=xx<=1
            template <typename T, const unsigned N> static inline
            T closest(const float xx, const T arr[]) throw()
            {
                assert(xx>=0.0f);
                assert(xx<=1.0f);
                return arr[ unsigned( floorf(xx*(N-1)+0.5f) ) ];
            }

            template <typename T, const unsigned N> static inline
            T linear(const float xx, const T arr[]) throw()
            {
                const float    xs    = xx * (N-1);
                unsigned       jm    = unsigned( floorf(xs+0.5f) );
                if(jm>=N-1)    jm    = N-2;
                const unsigned jp    = jm+1;
                const float    alpha = jp-xs;
                return blend<float,T>::mix(alpha,arr[jm],arr[jp]);
            }
        };

    }

}

#endif
