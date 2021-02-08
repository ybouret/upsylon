#include "y/gfx/borders.hpp"

namespace upsylon
{
    namespace GFX
    {
        Borders:: ~Borders() throw()
        {
        }

        Borders:: Borders(const Borders &borders) :
        bottom( borders.bottom ),
        top(    borders.top    ),
        left(   borders.left   ),
        right(  borders.right  )
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
                    aliasing::_(bottom) = SubArea::CreatePointer(area,Position::Bottom);
                    if(area.h>1)
                    {
                        aliasing::_(top) = SubArea::CreatePointer(area,Position::Top);
                    }
                }

                if(area.h>2)
                {
                    aliasing::_(left) = SubArea::CreatePointer(area,Position::Left);
                    if(area.w>1)
                    {
                        aliasing::_(right) = SubArea::CreatePointer(area,Position::Right);
                    }
                }

            }
        }


    }

}


