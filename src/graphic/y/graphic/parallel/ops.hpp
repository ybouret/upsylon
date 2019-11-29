//! \file

#ifndef Y_GRAPHICS_PARALLEL_OPS_INCLUDED
#define Y_GRAPHICS_PARALLEL_OPS_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"
//#include "y/graphic/convert.hpp"
#include "y/graphic/pixmap.hpp"


namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            template <
            typename TARGET,
            typename SOURCE,
            typename PROC>
            struct Ops {

                const Tiles_ *tiles;
                TARGET       *target;
                const SOURCE *source;
                PROC         *proc;
                int           status;

                static inline void Run( void *args, parallel &ctx, lockable &access ) throw()
                {
                    Ops          &self   = *static_cast<Ops *>(args);
                    const Tile   &tile   = (*self.tiles)[ctx.rank];
                    TARGET       &target = (*self.target);
                    const SOURCE &source = (*self.source);
                    PROC         &proc   = (*self.proc);

                    const Point lower = tile.lower;
                    const Point upper = tile.upper;

                    for(unit_t y=upper.y;y>=lower.y;--y)
                    {
                        const typename SOURCE::RowType &src = source[y];
                        typename       TARGET::RowType &tgt = target[y];
                        for(unit_t x=upper.x;x>=lower.x;--x)
                        {
                            try { tgt[x] = proc( src[x] ); }
                            catch(...)
                            {
                                Y_LOCK(access);
                                self.status = -1;
                                return;
                            }
                        }
                    }

                }

            };

        }

        struct Ops
        {
            template <typename TARGET,
            typename SOURCE,
            typename PROC> static inline
            void Run(Tiles        &tiles,
                     TARGET       &target,
                     const SOURCE &source,
                     PROC         &proc )
            {
                Kernel::Ops<TARGET,SOURCE,PROC> Task = { &tiles, &target, &source, &proc, 0 };
                tiles.loop().run(Task.Run,&Task);
            }

        };


    }

}

#endif

