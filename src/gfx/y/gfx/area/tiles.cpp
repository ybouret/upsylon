#include "y/gfx/area/tiles.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace GFX
    {

        Tiles:: ~Tiles() throw()
        {
            clear();
        }

        void Tiles:: clear() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();

            while(count>0)
            {
                self_destruct::on( tile[--aliasing::_(count)] );

            }

            mgr.release_as(tile,tCount,tBytes);
        }

        Tiles:: Tiles(const Area  &area,
                      const size_t maxThreads) :
        Area(area),
        count(0),
        tCount(0),
        tBytes(0),
        tile(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            const size_t items = static_cast<size_t>(n);
            if(items<=0)
            {

            }
            else
            {
                const size_t cpus  = clamp<size_t>(1,maxThreads,items);
                tCount  = cpus;
                tBytes  = 0;
                tile    = mgr.acquire_as<Tile>(tCount,tBytes);
                try
                {
                    for(size_t rank=0;rank<cpus;++rank)
                    {
                        new (tile+count) Tile(area,cpus,rank);
                        aliasing::_(count)++;
                        //std::cerr << "s[" << rank << "]=" << tile[rank] << std::endl;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }

        }

        
        const Tile & Tiles:: operator[](const size_t rank) const throw()
        {
            assert(rank<count);
            return tile[rank];
        }

        
    }
}
