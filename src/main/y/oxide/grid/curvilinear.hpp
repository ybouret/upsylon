
// \file
#ifndef Y_OXIDE_CURVILINEAR_GRID_INCLUDED
#define Y_OXIDE_CURVILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"

namespace upsylon {

    namespace Oxide {

        template <typename COORD, typename T>
        class CurvilinearGrid : public Grid<COORD,T>
        {
        public:
            inline explicit CurvilinearGrid()
            {
            }

            inline virtual ~CurvilinearGrid() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearGrid);
        };

    }
}

#endif
