

#include "y/gfx/area/core.hpp"

namespace upsylon
{
    namespace GFX
    {

        Core:: ~Core() throw()
        {
        }

        Core:: Core(const Area &area) :
        core( area.hasCore() ? new SubArea(area,Position::Core) : NULL ),
        inner( Items(core) )
        {

        }
        

        std::ostream & operator<<(std::ostream &os, const Core &c)
        {
            os << "core.inner            : #" << c.inner << std::endl;
            os << "    |_area            : "  << c.core;
            return os;
        }
    }

}


