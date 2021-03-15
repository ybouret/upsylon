
#include "y/gfx/area/segment.hpp"

namespace upsylon
{
    namespace graphic
    {
        segment:: ~segment() throw()
        {
        }
        
        segment:: segment(const unit_t Xmin,
                          const unit_t Y,
                          const unit_t Xmax) throw() :
        xmin(Xmin),
        y(Y),
        xmax(Xmax),
        w((1+xmax)-xmin)
        {
            assert(w>0);
        }

        segment:: segment(const segment &other) throw() :
        xmin(other.xmin),
        y(other.y),
        xmax(other.xmax),
        w(other.w)
        {
        }
        


        coord segment:: head() const throw()
        {
            return coord(xmin,y);
        }

        coord segment:: tail() const throw()
        {
            return coord(xmax,y);
        }
    }
    
}
