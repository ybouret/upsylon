
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


                static inline void Run(void *args, parallel &ctx, lockable &) throw()
                {
                    assert(args);

                    Copy         &self   = *static_cast<Copy *>(args); assert(self.tiles); assert(self.source); assert(self.target);
                    //const Tile   &tile   = (*self.tiles)[ctx.rank];
                    const PIXMAP &source = (*self.source);
                    PIXMAP       &target = (*self.target);
                    unit_t        length = source->h;
                    unit_t        offset = 0;

                    ctx.split(length,offset);
                    const unit_t  bytes = source->scanline;
                    while(length-- > 0 )
                    {
                        memcpy( target->stdRow(offset)->addr, source->stdRow(offset)->addr, bytes );
                        ++offset;
                    }
                }
            };

        }

        template <typename PIXMAP>
        inline void PixmapCopy(PIXMAP       &target,
                               const PIXMAP &source,
                               Tiles        &tiles)
        {
            assert( target.sameSurfaceThan(source) );
            Kernel::Copy<PIXMAP> Ops = { &tiles, &source, &target };
            tiles.loop().run( Kernel::Copy<PIXMAP>::Run, &Ops );
        }

    }
}

#endif
