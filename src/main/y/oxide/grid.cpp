#include "y/oxide/grid.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Oxide {

        void Grid_:: ExceptionLEQZ(const char *gridName,
                                   const char *context,
                                   const unsigned dim)
        {
            assert(gridName);
            assert(context);
            throw exception("%s: %s#%u<=0", gridName, context,dim);
        }

        const char * Grid_:: GetAxisName(const char **names, const size_t dim) throw()
        {
            if(names)
            {
                return names[dim];
            }
            else
            {
                return Coord::DefaultAxisName(dim);
            }
        }
    }

}

