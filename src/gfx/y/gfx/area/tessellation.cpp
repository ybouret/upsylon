#include "y/gfx/area/tessellation.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace graphic
    {
        tessellation:: ~tessellation() throw()
        {}
        
    
        tessellation:: tessellation(const area  &a,
                                    const size_t n) :
        entity(),
        area(a),
        tiles_type( clamp<size_t>(1,n,items) )
        {
            const size_t sz = count;
            for(size_t rk=0;rk<sz;++rk)
            {
                const arc_ptr<tile> t = new tile(*this,sz,rk);
                this->push(t);
            }
        }
        
    }
    
}


