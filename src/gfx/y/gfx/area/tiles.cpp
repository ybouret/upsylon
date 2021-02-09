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
                self_destruct( stretch[--aliasing::_(size)] );

            }

            mgr.release_as<Stretch>(stretch,count,bytes);
        }

        Tiles:: Tiles(const size_t             cores,
                      const Topology::Pointer &topo) :
        topology(topo),
        size(0),
        count(0),
        bytes(0),
        stretch(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            const size_t items = static_cast<size_t>(topology->inner);
            if(items<=0)
            {

            }
            else
            {
                const size_t cpus  = clamp<size_t>(1,cores,items);
                count   = cpus;
                bytes   = 0;
                stretch = mgr.acquire_as<Stretch>(count,bytes);

                try
                {
                    for(size_t rank=0;rank<cpus;++rank)
                    {
                        new (stretch+size) Stretch(*topology,cpus,rank);
                        aliasing::_(size)++;
                        std::cerr << "s[" << rank << "]=" << stretch[rank] << std::endl;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }

        }

        
        const Stretch & Tiles:: operator[](const size_t rank) const throw()
        {
            assert(rank<size);
            return stretch[rank];
        }

    }
}
