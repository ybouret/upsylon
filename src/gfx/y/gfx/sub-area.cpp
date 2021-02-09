#include "y/gfx/sub-area.hpp"

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
