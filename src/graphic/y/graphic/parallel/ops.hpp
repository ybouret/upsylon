//! \file

#ifndef Y_GRAPHICS_PARALLEL_OPS_INCLUDED
#define Y_GRAPHICS_PARALLEL_OPS_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/pixmap.hpp"
#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            //! default parallel operator on tiles
            template <
            typename TARGET,
            typename SOURCE,
            typename PROC>
            struct Ops {

                Tiles_       *tiles;   //!< original tiles
                TARGET       *target;  //!< target something
                const SOURCE *source;  //!< source something
                PROC         *proc;    //!< pixel wise op
                int           status;  //!< record error
                const SOURCE *source2; //!< for binary ops

                //! perform PROC(pixel) on each pixel of a Tile
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

                //! perform PROC(pixel,pixel2) on each pixel of a Tile
                static inline void RunBinary( void *args, parallel &ctx, lockable &access ) throw()
                {
                    Ops          &self    = *static_cast<Ops *>(args);
                    const Tile   &tile    = (*self.tiles)[ctx.rank];
                    TARGET       &target  = (*self.target);
                    const SOURCE &source  = (*self.source);
                    const SOURCE &source2 = (*self.source2);
                    PROC         &proc    = (*self.proc);

                    const Point lower = tile.lower;
                    const Point upper = tile.upper;

                    for(unit_t y=upper.y;y>=lower.y;--y)
                    {
                        const typename SOURCE::RowType &src  = source[y];
                        const typename SOURCE::RowType &src2 = source2[y];
                        typename       TARGET::RowType &tgt  = target[y];
                        for(unit_t x=upper.x;x>=lower.x;--x)
                        {
                            try { tgt[x] = proc( src[x], src2[x] ); }
                            catch(...)
                            {
                                Y_LOCK(access);
                                self.status = -1;
                                return;
                            }
                        }
                    }
                }


                //! perform PROC(pixel,tile) on each pixel of a Tile
                static inline void RunWithContext( void *args, parallel &ctx, lockable &access ) throw()
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
                            try { tgt[x] = proc( src[x],ctx ); }
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

        //! parallel ops wrapper
        struct Ops
        {
            //! prepare data and call the loop
            template <
            typename TARGET,
            typename SOURCE,
            typename PROC> static inline
            int Run(Tiles        &tiles,
                    TARGET       &target,
                    const SOURCE &source,
                    PROC         &proc )
            {
                Kernel::Ops<TARGET,SOURCE,PROC> Task = { &tiles, &target, &source, &proc, 0, NULL };
                tiles.loop().run(Task.Run,&Task);
                return Task.status;
            }


            //! prepare data and call the loop
            template <
            typename TARGET,
            typename SOURCE,
            typename PROC> static inline
            int RunWithContext(Tiles        &tiles,
                               TARGET       &target,
                               const SOURCE &source,
                               PROC         &proc )
            {
                Kernel::Ops<TARGET,SOURCE,PROC> Task = { &tiles, &target, &source, &proc, 0, NULL};
                tiles.loop().run(Task.RunWithContext,&Task);
                return Task.status;
            }


            //! prepare data and call the loop
            template <
            typename TARGET,
            typename SOURCE,
            typename PROC> static inline
            int RunBinary(Tiles        &tiles,
                          TARGET       &target,
                          const SOURCE &source,
                          const SOURCE &source2,
                          PROC         &proc )
            {
                Kernel::Ops<TARGET,SOURCE,PROC> Task = { &tiles, &target, &source, &proc, 0, &source2 };
                tiles.loop().run(Task.RunBinary,&Task);
                return Task.status;
            }


            //! call Run with built-in conversion
            template <
            typename TARGET,
            typename SOURCE> static inline
            int Convert(Tiles &tiles, TARGET &target, const SOURCE &source )
            {
                return Run(tiles,target,source, Convert::Get< typename TARGET::type, typename SOURCE::type> );
            }

            //! identity
            template <typename T> static inline
            const T & Id(const T&arg) throw() { return arg; }

            //! call Run with identity
            template <
            typename TARGET,
            typename SOURCE> static inline
            int Copy(Tiles &tiles, TARGET &target, const SOURCE &source )
            {
                return Run(tiles,target,source, Id<typename TARGET::type> );
            }

            //! distance
            template <typename T> static inline
            int Diff( Tiles &tiles, Pixmap<float> &diff, const Pixmap<T> &lhs, const Pixmap<T> &rhs )
            {
                return RunBinary(tiles, diff, lhs, rhs, Convert::Diff<T> );
            }

        };


    }

}

#endif

