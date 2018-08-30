//! \file
#ifndef Y_INK_STENCIL_SCHARR_INCLUDED
#define Y_INK_STENCIL_SCHARR_INCLUDED 1

#include "y/ink/stencil.hpp"

namespace upsylon
{
    namespace Ink
    {
#define Y_INK_SCHARR3X_DATA static const T mtx[9] = { -3, 0, 3, -10, 0, 10, -3, 0, 3 };
        Y_INK_STENCIL_IMPL(Scharr3X,
                           Y_INK_SCHARR3X_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

# define Y_INK_SCHARR3Y_DATA static const T mtx[9] = { -3, -10, -3, 0, 0, 0, 3, 10, 3 };
        Y_INK_STENCIL_IMPL(Scharr3Y,
                           Y_INK_SCHARR3Y_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

#define Y_INK_SCHARR5X_DATA \
static const T mtx[25] = {  \
-1,-1,0,1,1, \
-2,-2,0,2,2, \
-3,-6,0,6,3, \
-2,-2,0,2,2, \
-1,-1,0,1,1 \
}
        Y_INK_STENCIL_IMPL(Scharr5X,
                           Y_INK_SCHARR5X_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

#define Y_INK_SCHARR5Y_DATA \
static const T mtx[25] = {  \
-1,-2,-3,-2,-1,             \
-1,-2,-6,-2,-1,             \
0,0,0,0,0,                  \
1,2,6,2,1,                  \
1,2,3,2,1                   \
}
        Y_INK_STENCIL_IMPL(Scharr5Y,
                           Y_INK_SCHARR5Y_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );
    }
}

#endif

