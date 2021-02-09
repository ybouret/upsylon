#include "y/gfx/area/sub.hpp"

namespace upsylon
{
    namespace GFX
    {
        SubArea:: ~SubArea() throw()
        {

        }

        SubArea:: SubArea(const Area &area, const unsigned flag) :
        object(),
        Area( area.get(flag) ),
        position(flag)
        {
        }

        
    }
}
