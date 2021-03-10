#include "y/gfx/area.hpp"
#include "y/exception.hpp"
#include "y/type/standard.hpp"

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

        coord area:: coord_of(const unit_t i) const throw()
        {
            const core::standard<unit_t>::div_type l =  core::standard<unit_t>::div_call(i,w);
            return coord( lower.x + l.rem, lower.y+l.quot );
        }

        unit_t  area:: index_of(coord p) const throw()
        {
            p-=lower;
            return p.x + p.y * w;
        }

        unit_t  area:: index_of(const unit_t X, const unit_t Y) const throw()
        {
            return index_of( coord(X,Y) );
        }

        bool area:: has_same_metrics_than(const area &other) const throw()
        {
            return (lower == other.lower) && (upper == other.upper);
        }


    }
    
}

