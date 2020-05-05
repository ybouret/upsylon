//! \file

#ifndef Y_SPADE_TYPES_INCLUDED
#define Y_SPADE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon {
 
    namespace Spade
    {
        typedef unit_t           Coord1D;
        typedef point2d<Coord1D> Coord2D;
        typedef point3d<Coord1D> Coord3D;
        
        template <typename COORD>
        struct DimOf { static const size_t Value = sizeof(COORD)/sizeof(Coord1D); };
        
        
    }
    
}

#endif

