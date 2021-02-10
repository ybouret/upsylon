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

            while(size>0)
            {
                self_destruct( tile[--aliasing::_(size)] );

            }

            mgr.release_as(tile,count,bytes);
        }

        Tiles:: Tiles(const Area  &area,
                      const size_t maxThreads) :
        Area(area),
        size(0),
        count(0),
        bytes(0),
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
                count   = cpus;
                bytes   = 0;
                tile    = mgr.acquire_as<Tile>(count,bytes);
                try
                {
                    for(size_t rank=0;rank<cpus;++rank)
                    {
                        new (tile+size) Tile(area,cpus,rank);
                        aliasing::_(size)++;
                        std::cerr << "s[" << rank << "]=" << tile[rank] << std::endl;
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
            assert(rank<size);
            return tile[rank];
        }

        Tiles * Tiles:: Create(const Area &area, const size_t maxThreads)
        {
            return new Tiles(area,maxThreads);
        }


    }
}
