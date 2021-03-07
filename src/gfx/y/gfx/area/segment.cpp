
#include "y/gfx/area/segment.hpp"

namespace upsylon
{
    namespace graphic
    {
        segment:: ~segment() throw()
        {
        }
        
        segment:: segment(const coord c,const unit_t n) throw() :
        origin(c),
        length(n)
        {
        }
    }
    
}
