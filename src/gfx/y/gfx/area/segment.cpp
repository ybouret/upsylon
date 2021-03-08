
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
    }
    
}
