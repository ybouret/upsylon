
#include "y/gfx/area/tiles.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace graphic
    {
        tiles:: ~tiles() throw()
        {}


        tiles:: tiles(const area  &a,
                      const size_t n) :
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


