
//! \file

#ifndef Y_GRAPHIC_MERGE_INCLUDED
#define Y_GRAPHIC_MERGE_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/parallel/tiles.hpp"

namespace upsylon {

    namespace Graphic {

        //! merge components
        struct Merge
        {
            //! merge components, number of sources=channel.size()
            template <typename TYPE,typename T> static inline
            void Run(Pixmap<TYPE>             &target,
                     const Pixmap<T>          *sources,
                     const accessible<size_t> &channels,
                     Tiles                    &tiles)
            {
                checkChannels(channels, sizeof(TYPE)/sizeof(T) );
                struct Task
                {
                    Pixmap<TYPE>             *target_;
                    const Pixmap<T>          *sources_;
                    Tiles                    *tiles_;
                    const accessible<size_t> *channels_;

                    static inline
                    void Perform(void *args, parallel &ctx, lockable &) throw()
                    {
                        Task &task = *static_cast<Task *>(args);
                        Tile &tile = (*task.tiles_)[ctx.rank];
                        Run<TYPE,T>( *task.target_, task.sources_, *task.channels_, tile );
                    }
                };

                Task task = { &target, sources, &tiles, &channels };
                tiles.loop().run( Task::Perform, &task );
            }


        private:
            static void checkChannels( const accessible<size_t> &, const size_t dims);

            template <typename TYPE,typename T> static inline
            void Run(Pixmap<TYPE>             &target,
                     const Pixmap<T>          *sources,
                     const accessible<size_t> &channels,
                     const Tile               &tile)
            {
                const size_t nch = channels.size();
                const Point  lo  = tile.lower;
                const Point  up  = tile.upper;
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    typename Pixmap<TYPE>::RowType &target_y = target[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        TYPE     &tgt_ = target_y[x];
                        T        *tgt  = (T *) &tgt_;
                        for(size_t i=nch;i>0;--i)
                        {
                            const size_t j   = channels[i];
                            tgt[j] = sources[j][y][x];
                        }
                    }
                }
            }
        };

    }

}

#endif

