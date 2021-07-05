
#include "y/gfx/draw/clip.hpp"

namespace  upsylon {

    namespace graphic {

        namespace draw {

            size_t clip:: status_of( const unit_t x, const unit_t y, const area &a ) throw()
            {
                size_t flag = Inside;
                if(x<a.lower.x)
                {
                    flag |= Left;
                }
                else if(x>a.upper.x)
                {
                    flag |= Right;
                }

                if(y<a.lower.y)
                {
                    flag |= Bottom;
                }
                else if(y>a.upper.y)
                {
                    flag |= Top;
                }

                return flag;
            }


            bool clip:: accept(unit_t      &x0,
                               unit_t      &y0,
                               unit_t      &x1,
                               unit_t      &y1,
                               const area  &a ) throw()
            {
                Y_GFX_AREA_LIMITS(a);

                size_t code0 = status_of(x0,y0,a);
                size_t code1 = status_of(x1,y1,a);
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
                        code0 = status_of(x0,y0,a);
                    }
                    else
                    {
                        x1=x;
                        y1=y;
                        code1 = status_of(x1,y1,a);
                    }
                }
            }
        }
    }
}
