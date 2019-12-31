
//! \file
#ifndef Y_GRAPHIC3x3_INCLUDED
#define Y_GRAPHIC3x3_INCLUDED

#include "y/graphic/pixel.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/parallel/tiles.hpp"

namespace upsylon {

    namespace Graphic {

        //! 3x3 local filters
        struct Apply3x3
        {

            //! collect zerof flux points
#define Y_GRAPHIC3x3_COLLECT(I) do { const Point probe = org + Pixel::delta[I]; arr[I+1] = source(probe); } while(false)

            //! apply a procedure on a tile
            template <typename T,
            typename PROC> static inline
            void On(Pixmap<T>       &target,
                    const Pixmap<T> &source,
                    PROC            &proc,
                    const Tile      &tile )
            {
                char        data[ 16 * sizeof(T) ];
                T          *arr = (T *)data;
                const Point up = tile.upper;
                const Point lo = tile.lower;

                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    typename Pixmap<T>::RowType       &tgt = target[y];
                    const typename Pixmap<T>::RowType &src = source[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        arr[0] = src[x];
                        const Point org(x,y);
                        Y_GRAPHIC3x3_COLLECT(0);
                        Y_GRAPHIC3x3_COLLECT(1);
                        Y_GRAPHIC3x3_COLLECT(2);
                        Y_GRAPHIC3x3_COLLECT(3);
                        Y_GRAPHIC3x3_COLLECT(4);
                        Y_GRAPHIC3x3_COLLECT(5);
                        Y_GRAPHIC3x3_COLLECT(6);
                        Y_GRAPHIC3x3_COLLECT(7);
                        tgt[x] = proc(arr);
                    }
                }
            }

            //! parallel application
            template <typename T,typename PROC> static inline
            void On(Pixmap<T>       &target,
                    const Pixmap<T> &source,
                    PROC            &proc,
                    Tiles           &tiles)
            {
                struct Task
                {
                    Pixmap<T>       *target_;
                    const Pixmap<T> *source_;
                    PROC            *proc_;
                    const Tiles     *tiles_;

                    static inline
                    void Run( void *args, parallel &ctx, lockable &) throw()
                    {
                        Task       &task = *static_cast<Task *>(args);
                        const Tile &tile = (*task.tiles_)[ctx.rank];
                        On( *(task.target_), (*task.source_), (*task.proc_), tile);
                    }
                };
                Task task = { &target, &source, &proc, &tiles };
                tiles.loop().run( Task::Run, &task);
            }

            //! replace value by its median
            template <typename T> static inline
            void Median( Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles )
            {
                Apply3x3::On(target,source, Pixel::Median9<T>, tiles);
            }

            //! replace value by its average
            template <typename T> static inline
            void Average( Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles )
            {
                Apply3x3::On(target,source, Pixel::Average9<T>, tiles);
            }

            //! replace value by the its minimum
            template <typename T> static inline
            void Erode( Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles )
            {
                Apply3x3::On(target,source, Pixel::MinOf9<T>, tiles);
            }

            //! replace value by its maxium
            template <typename T> static inline
            void Dilate( Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles )
            {
                Apply3x3::On(target,source, Pixel::MaxOf9<T>, tiles);
            }

            //! erode and dilate
            template <typename T> static inline
            void Open(Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles, Pixmap<T> &tmp )
            {
                Erode(tmp,source,tiles);
                Dilate(target,tmp,tiles);
            }

            //! dilate then erode
            template <typename T> static inline
            void Close(Pixmap<T> &target, const Pixmap<T> &source, Tiles &tiles, Pixmap<T> &tmp )
            {
                Dilate(tmp,source,tiles);
                Erode(target,tmp,tiles);
            }

        };

    }
}

#endif

