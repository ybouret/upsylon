
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {


        static inline
        Edges::LocalMaxima & getLocalMaxima( const Tile &tile ) throw()
        {
            assert( tile.localMemory() >= Edges:: LocalBytesPerTile );
            const size_t     *H   = & tile.as<size_t>(0);
            return *(Edges::LocalMaxima *) &H[ Histogram::BINS ];
        }

        void Edges:: keepLocalMaxima( Tile &tile ) throw()
        {
            assert( tile.localMemory() >=  LocalBytesPerTile );
            const Point up = tile.upper;
            const Point lo = tile.lower;
            if(gmax>0.0f)
            {
                size_t     *H   = & tile.as<size_t>(0);
                LocalMaxima &lm = *(Edges::LocalMaxima *) &H[ Histogram::BINS ];
                assert(0==lm.count);
                assert(0==lm.points);
                lm.points       = & P[lo];
                const float g2l = 255.0f/gmax;
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    const Pixmap<Vertex>::RowType &Gy = G[y];
                    const Pixmap<float>::RowType  &gy = g[y];
                    Pixmap<uint8_t>::RowType      &Ly = L[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        const Point  org(x,y);
                        const Vertex v = Gy[x];
                        const Point  delta( unit_t( floorf(v.x+0.5f) ), unit_t( floorf(v.y+0.5f) ) );
                        const Point  prev = org-delta;
                        const Point  next = org+delta;
                        const float  g0   = gy[x];
                        const float  gm   = g(prev);
                        const float  gp   = g(next);
                        if(gm<=g0 && gp<=g0 )
                        {
                            const uint8_t u = max_of<uint8_t>( uint8_t( floorf( g0*g2l + 0.5f) ), 1);
                            Ly[x] = u;
                            ++H[u];
                            lm.points[ lm.count++ ] = Point(x,y);
                        }
                        else
                        {
                            Ly[x] = 0;
                        }
                    }
                }
            }
            else
            {
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    Pixmap<uint8_t>::RowType &Ly = L[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        Ly[x] = 0;
                    }
                }
            }
        }

        size_t Edges:: keepLocalMaxima( Tiles      &tiles )
        {
            tiles.localAcquire( LocalBytesPerTile );
            tiles.localCleanUp();
            struct Task
            {
                Edges *self;
                Tiles *tiles;

                static void Run( void *args, parallel &ctx, lockable &) throw()
                {
                    Task &task = *static_cast<Task *>(args);
                    task.self->keepLocalMaxima( (*task.tiles)[ctx.rank] );
                }

            };

            Task task = { this, &tiles };
            tiles.loop().run( Task::Run, &task);
            hist.epilog(tiles);

            const size_t nt     = tiles.size();
            size_t       length = getLocalMaxima(tiles[0]).count;
            Point       *target = &P[0][0] + length;
            for(size_t i=1;i<nt;++i)
            {
                const LocalMaxima &lm = getLocalMaxima(tiles[i]);
                const size_t       np = lm.count;
                memmove(target,lm.points,np*sizeof(Point));
                target += np;
                length += np;
            }
            return length;
        }

    }

}


