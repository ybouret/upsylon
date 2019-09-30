#include "y/oxide/grid.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Oxide {

        void Grid_:: LEQZ(const char *gridName,
                          const char *context,
                          const unsigned dim)
        {
            assert(gridName);
            assert(context);
            throw exception("%s: %s#%u<=0", gridName, context,dim);
        }
    }

}

