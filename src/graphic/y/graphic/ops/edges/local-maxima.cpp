
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {


        void Edges:: keepLocalMaxima( Tile &tile ) throw()
        {
            assert( tile.localMemory() >=  LocalHistogramBytes );
            const Point up = tile.upper;
            const Point lo = tile.lower;
            if(gmax>0.0f)
            {
                size_t     *H   = & tile.as<size_t>(0);
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
                        uint8_t     &l    = Ly[x];
                        l = 0;
                        if(g0>0.0f)
                        {
                            const float  gm   = g(prev);
                            const float  gp   = g(next);
                            if( (gm<g0) && (gp<g0) )
                            {
                                const uint8_t u = uint8_t( floorf( g0*g2l + 0.5f) );
                                Ly[x] = u;
                                ++H[u];
                                std::cerr << "local: " << g0 << "/" << gmax << "@" << x << "," << y << std::endl;
                            }
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

        void Edges:: keepLocalMaxima( Tiles      &tiles )
        {
            tiles.localAcquire( LocalHistogramBytes );
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
            
        }

    }

}


