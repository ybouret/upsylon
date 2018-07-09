
#include "y/memory/vein.hpp"

namespace upsylon
{
    namespace memory
    {
        vein:: vein() throw() :
        entry(0),
        workspace()
        {
            std::cerr << "sizeof(workspace)=" << sizeof(workspace) << std::endl;
        }

        vein:: ~vein() throw()
        {
        }

    }
}
