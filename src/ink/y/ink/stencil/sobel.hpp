//! \file
#ifndef Y_INK_STENCIL_SOBEL_INCLUDED
#define Y_INK_STENCIL_SOBEL_INCLUDED 1

#include "y/ink/stencil.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! inline data
#define Y_INK_SOBEL3X_DATA static const T mtx[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

        //! Sobel 3x3 along X
        Y_INK_STENCIL_IMPL(Sobel3X,
                           Y_INK_SOBEL3X_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

        //! inline data
#define Y_INK_SOBEL3Y_DATA static const T mtx[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 }

        //! Sobel 3x3 along Y
        Y_INK_STENCIL_IMPL(Sobel3Y,
                           Y_INK_SOBEL3Y_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

        //! inline data
#define Y_INK_SOBEL5Y_DATA \
static const T mtx[25] = {\
-5, -8,-10, -8,-5,\
-4,-10,-20,-10,-4,\
0,  0,  0,  0,  0,\
4, 10, 20, 10,  4,\
5,  8, 10,  8,  5 \
}

        //! Sobel 5x5 along X
        Y_INK_STENCIL_IMPL(Sobel5Y,
                           Y_INK_SOBEL5Y_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

        //! inline data
#define Y_INK_SOBEL5X_DATA \
static const T mtx[25] = { \
-5,-4,0,4,5,               \
-8,-10,0,10,8,             \
-10,-20,0,20,10,           \
-8,-10,0,10,8,             \
-5,-4,0,4,5,               \
}

        //! Sobel 5x5 along Y
        Y_INK_STENCIL_IMPL(Sobel5X,
                           Y_INK_SOBEL5X_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );
    }
}

#endif

