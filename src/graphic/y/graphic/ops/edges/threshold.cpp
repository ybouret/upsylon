
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {

        void Edges:: applyThresholds( Tile &tile ) throw()
        {
            const Point   up   = tile.upper;
            const Point   lo   = tile.lower;
            const uint8_t hard = hardThreshold;
            const uint8_t soft = softThreshold;
            for(unit_t y=up.y;y>=lo.y;--y)
            {
                Pixmap<uint8_t>::RowType      &Ly = L[y];
                for(unit_t x=up.x;x>=lo.x;--x)
                {
                    uint8_t       &l = Ly[x];
                    const uint8_t  u = l;
                    if( u>=hard )
                    {
                        l = HARD;
                    }
                    else if( u>=soft )
                    {
                        l = SOFT;
                    }
                    else
                    {
                        l = 0;
                    }
                }

            }
        }

        void Edges:: applyThresholds( Tiles &tiles )
        {
            // compute thresholds
            hardThreshold = hist.Otsu1D();
            softThreshold = hardThreshold>>1;

            std::cerr << "Threshold: " << int(hardThreshold) << ":" << int(softThreshold) << std::endl;

            // run in parallel
            struct Task
            {
                Edges *self;
                Tiles *tiles;

                static void Run( void *args, parallel &ctx, lockable &) throw()
                {
                    Task &task = *static_cast<Task *>(args);
                    task.self->applyThresholds( (*task.tiles)[ctx.rank] );
                }

            };

            Task task = { this, &tiles };
            tiles.loop().run( Task::Run, &task);


        }


    }

}

