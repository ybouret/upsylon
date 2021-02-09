#include "y/gfx/area/borders.hpp"

namespace upsylon
{
    namespace GFX
    {
        Borders:: ~Borders() throw()
        {
        }


        unit_t Borders:: items() const throw()
        {
            unit_t sum = 0;
            sum += Items(bottom);
            sum += Items(top);
            sum += Items(left);
            sum += Items(right);
            return sum;
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
        }



        std::ostream & operator <<(std::ostream &os, const Borders &borders)
        {
            std::cerr << "borders.items         : " << borders.Borders::items() << std::endl;
            std::cerr << "       |_bottom       : " << borders.bottom     << std::endl;
            std::cerr << "       |_top          : " << borders.top        << std::endl;
            std::cerr << "       |_left         : " << borders.left       << std::endl;
            std::cerr << "       |_right        : " << borders.right;
            return os;
        }

    }

}


