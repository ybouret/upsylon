#include "y/gfx/sub-area.hpp"

namespace upsylon
{
    namespace GFX
    {
        SubArea:: ~SubArea() throw()
        {

        }

        SubArea:: SubArea(const Area &area, const unsigned flag) :
        Object(),
        Area( area.get(flag) ),
        position(flag)
        {
        }

        SubArea:: SubArea(const SubArea &sub) throw() :
        Object(),
        Area(sub),
        position(sub.position)
        {

        }
        

    }
}
