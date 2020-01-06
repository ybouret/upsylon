//! \file

#ifndef Y_GRAPHIC_SPLIT_INCLUDED
#define Y_GRAPHIC_SPLIT_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/parallel/tiles.hpp"

namespace upsylon {

    namespace Graphic {


        struct Split
        {


            template <typename TYPE,typename T> static inline
            void Run(Pixmap<T>                *targets,
                     const Pixmap<TYPE>       &source,
                     const accessible<size_t> &channels,
                     Tiles                     &tiles)
            {
                checkChannels(channels, sizeof(TYPE)/sizeof(T) );

                struct Task
                {
                    Pixmap<T>                *targets_;
                    const Pixmap<TYPE>       *source_;
                    Tiles                    *tiles_;
                    const accessible<size_t> *channels_;

                    static inline
                    void Perform(void *args, parallel &ctx, lockable &) throw()
                    {
                        Task &task = *static_cast<Task *>(args);
                        Tile &tile = (*task.tiles_)[ctx.rank];
                        Run<TYPE,T>( task.targets_, *task.source_,  *task.channels_, tile );
                    }
                };

                Task task = { targets, &source, &tiles, &channels };
                tiles.loop().run( Task::Perform, &task );
            }

        private:
            static void checkChannels( const accessible<size_t> &channels, const size_t dims );

            template <typename TYPE,typename T> static
            void Run(Pixmap<T>                *targets,
                     const Pixmap<TYPE>       &source,
                     const accessible<size_t> &channels,
                     const Tile               &tile)
            {
                const size_t nch = channels.size();
                const Point  lo  = tile.lower;
                const Point  up  = tile.upper;
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    const typename Pixmap<TYPE>::RowType &source_y = source[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        const TYPE &src_ = source_y[x];
                        const T    *src  = (const T *) &src_;
                        for(size_t i=nch;i>0;--i)
                        {
                            const size_t j   = channels[i];
                            targets[j][y][x] = src[j];
                        }
                    }
                }
            }




        };


    }

}

#endif



