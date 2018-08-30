//! \file
#ifndef Y_INK_STENCIL_DELTA_INCLUDED
#define Y_INK_STENCIL_DELTA_INCLUDED 1

#include "y/ink/stencil.hpp"

namespace upsylon
{
    namespace Ink
    {

        Y_INK_STENCIL_IMPL(DeltaX,
                           weights.make(1,3);
                           weights[1][1] = -1;
                           weights[1][3] =  1;
                           );

        Y_INK_STENCIL_IMPL(DeltaY,
                           weights.make(3,1);
                           weights[1][ 1] = -1;
                           weights[3][ 1] =  1;
                           );

    }
}

#endif

