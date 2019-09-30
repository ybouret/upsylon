
// \file
#ifndef Y_OXIDE_STRUCTURED_GRID_INCLUDED
#define Y_OXIDE_STRUCTURED_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"

namespace upsylon {

    namespace Oxide {

        template <typename COORD, typename T>
        class StructuredGrid : public Grid<COORD,T>
        {
        public:
            inline explicit StructuredGrid()
            {
            }

            inline virtual ~StructuredGrid() throw()
            {
            }

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(StructuredGrid);
        };


    }
}

#endif
