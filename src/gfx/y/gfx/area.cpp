#include "y/gfx/area.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    
    namespace gfx
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
        
    }
    
}

