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

        SubArea:: SubArea(const SubArea &sub) throw() :
        object(),
        Area(sub),
        position(sub.position)
        {

        }



        SubArea:: Holder:: ~Holder() throw() {}
        SubArea:: Holder::  Holder() throw() : count(0) {}

    }
}
