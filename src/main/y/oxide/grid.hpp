// \file
#ifndef Y_OXIDE_GRID_INCLUDED
#define Y_OXIDE_GRID_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon {

    namespace Oxide {

        template <typename COORD, typename T>
        class Grid
        {
        public:
            inline virtual ~Grid() throw() {}

        protected:
            inline explicit Grid() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grid);
        };

    }
}

#endif

