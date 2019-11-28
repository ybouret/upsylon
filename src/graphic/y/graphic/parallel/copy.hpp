
//! \file

#ifndef Y_GRAPHIC_PARALLEL_COPY_INCLUDED
#define Y_GRAPHIC_PARALLEL_COPY_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/pixmap.hpp"

namespace upsylon {

    namespace Graphic {


        namespace Kernel {

            template <typename PIXMAP>
            struct Copy
            {
                const Tiles_ *tiles;
                const PIXMAP *source;
                PIXMAP       *target;

                static inline void Run(void *args, parallel &ctx, lockable &l) throw()
                {
                    assert(args);


                    Copy         &self   = *static_cast<Copy *>(args); assert(self.tiles); assert(self.source); assert(self.target);
                    const Tile   &tile   = (*self.tiles)[ctx.rank];
                    const PIXMAP &source = (*self.source);
                    PIXMAP       &target = (*self.target);
                    const Point   lower  = tile.lower;
                    const Point   upper  = tile.upper;
                    if(false) {
                        Y_LOCK(l);
                        std::cerr << "in rank=" << ctx.rank << ": " << lower << "->" << upper << " : " << tile.items << "/" << source->items << std::endl;
                    }
                    size_t n = 0;
                    for(unit_t j=upper.y;j>=lower.y;--j)
                    {
                        typename PIXMAP::RowType       &target_j = target[j];
                        const typename PIXMAP::RowType &source_j = source[j];
                        for(unit_t i=upper.x;i>=lower.x;--i)
                        {
                            target_j[i] = source_j[i];
                            ++n;
                        }
                    }
                    assert(n==tile.items);
                }
            };

        }

        template <typename PIXMAP>
        inline void PixmapCopy(PIXMAP       &target,
                               const PIXMAP &source,
                               Tiles        &tiles)
        {
            Kernel::Copy<PIXMAP> Ops = { &tiles, &source, &target };
            tiles.run( Kernel::Copy<PIXMAP>::Run, &Ops );
        }

    }
}

#endif
