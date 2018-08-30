//! \file
#ifndef Y_INK_STENCIL_DELTA_INCLUDED
#define Y_INK_STENCIL_DELTA_INCLUDED 1

#include "y/ink/stencil.hpp"

namespace upsylon
{
    namespace Ink
    {

        Y_INK_STENCIL_IMPL(DeltaX,
                           m.make(1,3);
                           m[1][1] = -1; m[1][2] = 0; m[1][3] =  1;);

        Y_INK_STENCIL_IMPL(DeltaY,
                           m.make(3,1);
                           m[1][ 1] = -1;
                           m[2][ 1] =  0;
                           m[3][ 1] =  1);

        Y_INK_STENCIL_IMPL(Laplacian,
                           m.make(3,3);
                           m[1][1] = 0; m[1][2] =  1; m[1][3] = 0;
                           m[2][1] = 1; m[2][2] = -4; m[3][1] = 1;
                           m[3][1] = 0; m[3][2] =  1; m[3][3] = 0);

    }
}

#endif

