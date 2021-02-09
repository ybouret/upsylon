#include "y/gfx/corners.hpp"

namespace upsylon
{
    namespace GFX
    {

        Corners:: ~Corners() throw()
        {
        }

        unit_t Corners:: items() const throw()
        {
            unit_t sum = 0;
            sum += Items(bottom_left);
            sum += Items(bottom_right);
            sum += Items(top_left);
            sum += Items(top_right);
            return sum;
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

                
            }
        }

        std::ostream & operator <<(std::ostream &os, const Corners &corners)
        {
            std::cerr << "corners.items         : " << corners.Corners::items() << std::endl;
            std::cerr << "       |_bottom_left  : " << corners.bottom_left  << std::endl;
            std::cerr << "       |_bottom_right : " << corners.bottom_right << std::endl;
            std::cerr << "       |_top_left     : " << corners.top_left     << std::endl;
            std::cerr << "       |_top_right    : " << corners.top_right;
            return os;
        }

    }

}

