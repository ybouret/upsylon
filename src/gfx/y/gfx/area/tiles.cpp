
#include "y/gfx/area/tiles.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace graphic
    {
        tiles:: ~tiles() throw()
        {}


        size_t tiles:: size_for(const area &a, const size_t n) throw()
        {
            return clamp<size_t>(1,n,a.items);
        }

        tiles:: tiles(const area  &a, const size_t n) :
        area(a), tiles_type( size_for(a,n) )
        {
            initialize();
        }

        void tiles:: initialize()
        {
            const size_t sz = count;
            for(size_t rk=0;rk<sz;++rk)
            {
                this->build<tile *>( new tile(*this,sz,rk) );
            }
        }

        tiles:: tiles(const area &a, const collection &c) :
        area(a), tiles_type( size_for(a,c.size()) )
        {
            initialize();
        }

    }

}


