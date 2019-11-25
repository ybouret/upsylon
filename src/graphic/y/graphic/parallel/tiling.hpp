//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILING_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILING_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        

        typedef point2d<size_t> Cores;

        class Tiling
        {
        public:

            // compute cores to tile area
            static Cores ComputeCoresFor( const Area &area, const size_t CPUs);
            static void  ComputeTiles( const Area &area, const size_t CPUs );


        private:
            
        };

    }
}

#endif

