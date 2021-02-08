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

                aliasing::_(bottom_left) = new SubArea(area,Position::Bottom|Position::Left);
                if(area.w>1)
                {
                    aliasing::_(bottom_right) = new SubArea(area,Position::Bottom|Position::Right);
                }

                if(area.h>1)
                {
                    aliasing::_(top_left) = new SubArea(area,Position::Top|Position::Left);
                    if(area.w>1)
                    {
                        aliasing::_(top_right) = new SubArea(area,Position::Top|Position::Right);
                    }
                }

                if(bottom_left.is_valid())  aliasing::_(count) += bottom_left->n;
                if(top_left.is_valid())     aliasing::_(count) += top_left->n;
                if(bottom_right.is_valid()) aliasing::_(count) += bottom_right->n;
                if(top_right.is_valid())    aliasing::_(count) += top_right->n;
            }
        }

    }

}

