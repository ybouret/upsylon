// \file
#ifndef Y_OXIDE_RECTILINEAR_GRID_INCLUDED
#define Y_OXIDE_RECTILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"

namespace upsylon {

    namespace Oxide {

        template <typename COORD, typename T>
        class RectilinearGrid : public Grid<COORD,T>
        {
        public:
            inline explicit RectilinearGrid()
            {
            }

            inline virtual ~RectilinearGrid() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearGrid);
        };


    }
}

#endif
