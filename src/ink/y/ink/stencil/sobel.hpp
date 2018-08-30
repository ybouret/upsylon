//! \file
#ifndef Y_INK_STENCIL_SOBEL_INCLUDED
#define Y_INK_STENCIL_SOBEL_INCLUDED 1

#include "y/ink/stencil.hpp"

namespace upsylon
{
    namespace Ink
    {
#define Y_INK_SOBELX3_DATA static const T mtx[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
        Y_INK_STENCIL_IMPL(SobelX3,
                           Y_INK_SOBELX3_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

#define Y_INK_SOBELY3_DATA static const T mtx[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 }
        Y_INK_STENCIL_IMPL(SobelY3,
                           Y_INK_SOBELY3_DATA;
                           m.make(3,3);
                           memcpy(*m,mtx,sizeof(mtx));
                           );


#define Y_INK_SOBELY5_DATA \
static const T mtx[25] = {\
-5, -8,-10, -8,-5,\
-4,-10,-20,-10,-4,\
0,  0,  0,  0,  0,\
4, 10, 20, 10,  4,\
5,  8, 10,  8,  5 \
}
        Y_INK_STENCIL_IMPL(SobelY5,
                           Y_INK_SOBELY5_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );

#define Y_INK_SOBELX5_DATA \
static const T mtx[25] = { \
-5,-4,0,4,5,               \
-8,-10,0,10,8,             \
-10,-20,0,20,10,           \
-8,-10,0,10,8,             \
-5,-4,0,4,5,               \
}

        Y_INK_STENCIL_IMPL(SobelX5,
                           Y_INK_SOBELX5_DATA;
                           m.make(5,5);
                           memcpy(*m,mtx,sizeof(mtx));
                           );
    }
}

#endif

