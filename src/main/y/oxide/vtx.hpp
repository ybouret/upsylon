
//! \file
#ifndef Y_OXIDE_VTX_INCLUDED
#define Y_OXIDE_VTX_INCLUDED 1

#include "y/type/point3d.hpp"


namespace upsylon {

    namespace Oxide
    {

        struct Vertex
        {
            template <typename VTX,typename T>
            struct Get
            {
                static const size_t Dimensions = sizeof(VTX)/sizeof(T);
            };
        };
    }

}

#endif

