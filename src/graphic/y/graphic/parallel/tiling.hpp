//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILING_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILING_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {


        typedef point2d<size_t> Cores; //!< alias

        //! Tiling functions
        struct Tiling
        {
            //! get the optimal 2D cores to tile an area
            static Cores ComputeCoresFor( const Area &area, const size_t CPUs);

        };

    }
}

#endif

