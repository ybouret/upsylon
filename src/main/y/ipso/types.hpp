//! \file
#ifndef Y_IPSO_TYPES_INCLUDED
#define Y_IPSO_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon
{
    namespace ipso
    {
        typedef unit_t           coord1D;
        typedef point2d<coord1D> coord2D;
        typedef point3d<coord1D> coord3D;
        template <typename COORD>
        struct dim_of
        {
            static const size_t value = sizeof(COORD)/sizeof(coord1D);
        };
    }
}

#endif

