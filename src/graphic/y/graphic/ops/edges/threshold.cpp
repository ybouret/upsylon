
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {

        void Edges:: applyThresholds( Tile &tile ) throw()
        {
            assert( tile.localMemory() >= LocalMaximaBytes );

            const Point    up     = tile.upper;
            const Point    lo     = tile.lower;
            const uint8_t  strong = strongThreshold;
            const uint8_t  feeble = feebleThreshold;
            LocalMaxima   &lm     = tile.as<LocalMaxima>();
            lm.count  = 0;
            lm.points = &P[0][0] + tile.linearOffset;
            for(unit_t y=up.y;y>=lo.y;--y)
            {
                Pixmap<uint8_t>::RowType      &Ly = L[y];
                for(unit_t x=up.x;x>=lo.x;--x)
                {
                    uint8_t       &l = Ly[x];
                    const uint8_t  u = l;
                    if( u>=strong )
                    {
                        l = STRONG;
                        goto MARK;
                    }
                    else if( u>=feeble )
                    {
                        l = FEEBLE;
                        goto MARK;
                    }
                    else
                    {
                        l = NoEDGE;
                        continue;
                    }

                MARK:
                    lm.points[ lm.count++ ] = Point(x,y);
                }

            }
        }

        size_t Edges:: applyThresholds( Tiles &tiles )
        {
            //------------------------------------------------------------------
            //
            // local memory
            //
            //------------------------------------------------------------------
            tiles.localAcquire( LocalMaximaBytes );

            //------------------------------------------------------------------
            //
            // compute thresholds
            //
            //------------------------------------------------------------------
            feebleThreshold = (strongThreshold = hist.Otsu1D()) >> 1;

            std::cerr << "Threshold: " << int(strongThreshold) << ":" << int(feebleThreshold) << std::endl;

            //------------------------------------------------------------------
            //
            // run in parallel
            //
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //
            // compactification of points
            //
            //------------------------------------------------------------------
            const size_t nt     = tiles.size();
            size_t       length = tiles[0].as<LocalMaxima>().count;
            Point       *target = &P[0][0] + length;
            for(size_t i=1;i<nt;++i)
            {
                const LocalMaxima &lm = tiles[i].as<LocalMaxima>();
                const size_t       np = lm.count;
                memmove( (void*)target, (const void *)lm.points,np*sizeof(Point));
                target += np;
                length += np;
            }

            return length;
        }


    }

}

