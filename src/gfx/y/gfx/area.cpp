#include "y/gfx/area.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        area:: area(const unit_t W, const unit_t H,
                    const unit_t X, const unit_t Y) :
        w( checking::geqz(W,checking::width)  ),
        h( checking::geqz(H,checking::height) ),
        items(w*h),
        lower(X,Y),
        upper(X+W-1,Y+H-1)
        {
            if( (w>0||h<=0) && (h>0&&w<=0) )
                throw exception("invalid area");
        }
        
        area:: ~area() throw()
        {
        }
        
        std::ostream & operator<<(std::ostream &os, const area &a)
        {
            os << "[" << a.w << "x" << a.h << "] : " << a.lower << "->" << a.upper << " #" << a.items;
            return os;
        }

        area:: area(const area &a) throw() :
        w(a.w),
        h(a.h),
        items(a.items),
        lower(a.lower),
        upper(a.upper)
        {
        }

        bool area:: owns(const unit_t x, const unit_t y) const throw()
        {
            return (x>=lower.x&&x<=upper.x) && (y>=lower.y&&y<=upper.y);
        }

        bool area:: owns(const coord p) const throw()
        {
            return owns(p.x,p.y);
        }

        bool area:: owns(const area &sub) const throw()
        {
            return owns(sub.lower) && owns(sub.upper);
        }


    }
    
}

