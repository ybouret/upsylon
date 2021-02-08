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

        SubArea::Pointer * SubArea:: CreatePointer(const Area &area, const unsigned flag)
        {
            auto_ptr<SubArea> s = new SubArea(area,flag);
            Pointer          *p = new Pointer( & *s );
            s.dismiss();
            return p;
        }

        SubArea:: Holder:: ~Holder() throw() {}

        SubArea:: Holder:: Holder() throw() : count(0) {}

    }
}
