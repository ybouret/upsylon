
#include "y/graphic/draw/clip.hpp"

namespace  upsylon {

    namespace Graphic {

        namespace Draw {

            size_t Clip:: StatusOf( const unit_t x, const unit_t y, const Box &area ) throw()
            {
                size_t flag = Inside;
                if(x<area.lower.x)
                {
                    flag |= Left;
                }
                else if(x>area.upper.x)
                {
                    flag |= Right;
                }

                if(y<area.lower.y)
                {
                    flag |= Bottom;
                }
                else if(y>area.upper.y)
                {
                    flag |= Top;
                }

                return flag;
            }


            bool Clip:: Accept(unit_t     &x0,
                               unit_t     &y0,
                               unit_t     &x1,
                               unit_t     &y1,
                               const Box  &area ) throw()
            {
                Y_GRAPHIC_BOX_LIMITS(area);

                size_t code0 = StatusOf(x0,y0,area);
                size_t code1 = StatusOf(x1,y1,area);
                while(true)
                {
                    // bitwise OR is 0: both points inside window
                    if( !(code0|code1) )
                    {
                        return true;
                    }
                    // bitwise AND != 0: both point outside same same part of outside
                    if ( 0 != (code0 & code1) )
                    {
                        return false;
                    }
                    assert(code0!=code1);
                    assert( (code0!=0) || (code1!=0) );

                    // at least one of the code is not null
                    const size_t code = code0 ? code0 : code1;
                    unit_t x=0,y=0;
                    if     (0 != (code&Top)     )
                    {
                        x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                        y = ymax;
                    }
                    else if( 0 != (code&Bottom) )
                    {
                        x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                        y = ymin;
                    }
                    else if( 0 != (code&Right) )
                    {
                        y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                        x = xmax;
                    }
                    else if( 0 != (code&Left) )
                    {
                        y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                        x = xmin;
                    }
                    if(code==code0)
                    {
                        x0 = x;
                        y0 = y;
                        code0 = StatusOf(x0,y0,area);
                    }
                    else
                    {
                        x1=x;
                        y1=y;
                        code1 = StatusOf(x1,y1,area);
                    }
                }
            }
        }
    }
}
