#include "y/gfx/borders.hpp"

namespace upsylon
{
    namespace GFX
    {
        Borders:: ~Borders() throw()
        {
        }



         
        Borders:: Borders(const Area &area) :
        bottom(NULL),
        top(NULL),
        left(NULL),
        right(NULL)
        {
            if(area.n>0)
            {

                if(area.w>2)
                {
                    aliasing::_(bottom) = new SubArea(area,Position::Bottom);
                    if(area.h>1)
                    {
                        aliasing::_(top) = new SubArea(area,Position::Top);
                    }
                }

                if(area.h>2)
                {
                    aliasing::_(left) = new SubArea(area,Position::Left);
                    if(area.w>1)
                    {
                        aliasing::_(right) = new SubArea(area,Position::Right);
                    }
                }

            }

            if(bottom.is_valid()) aliasing::_(count) += bottom->n;
            if(top.is_valid())    aliasing::_(count) += top->n;
            if(left.is_valid())   aliasing::_(count) += left->n;
            if(right.is_valid())  aliasing::_(count) += right->n;

        }


    }

}


