
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {

        Edges:: ~Edges() throw()
        {
        }

        Edges:: Edges( const size_t W, const size_t H) :
        gmax(0),
        g(W,H),
        G(W,H),
        L(W,H)
        {
        }


        void Edges:: keepLocalMaxima( const Tile &tile ) throw()
        {
            const Point up = tile.upper;
            const Point lo = tile.lower;
            for(unit_t y=up.y;y>=lo.y;--y)
            {
                for(unit_t x=up.x;x>=lo.x;--x)
                {
                    const Point  org(x,y);
                    const Vertex v = G[y][x];
                    const Point  delta( unit_t( floorf(v.x+0.5f) ), unit_t( floorf(v.y+0.5f) ) );
                    const Point  prev = org-delta;
                    const Point  next = org+delta;
                    const float  g0   = g[y][x];
                    const float  gm   = g(prev);
                    const float  gp   = g(next);
                    if(gm<=g0 && gp<=g0 )
                    {
                        L[y][x] = 1;
                    }
                    else
                    {
                        L[y][x] = 0;
                    }
                }
            }
        }

        void Edges:: keepLocalMaxima( Tiles      &tiles )
        {
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
        }

    }
}
