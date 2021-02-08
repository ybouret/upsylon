#include "y/gfx/corners.hpp"

namespace upsylon
{
    namespace GFX
    {

        Corners:: ~Corners() throw()
        {
        }

        Corners:: Corners(const Area &area) :
        bottom_left(0),
        bottom_right(0),
        top_left(0),
        top_right(0)
        {
            if(area.n>0)
            {
                aliasing::_(bottom_left) = SubArea::CreatePointer(area,Position::Bottom|Position::Left);
                if(area.w>1)
                {
                    aliasing::_(bottom_right) = SubArea::CreatePointer(area,Position::Bottom|Position::Right);
                }

                if(area.h>1)
                {
                    aliasing::_(top_left) = SubArea::CreatePointer(area,Position::Top|Position::Left);
                    if(area.w>1)
                    {
                        aliasing::_(top_right) = SubArea::CreatePointer(area,Position::Top|Position::Right);
                    }
                }

            }
        }

    }

}

