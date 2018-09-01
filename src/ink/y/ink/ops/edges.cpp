#include "y/ink/ops/edges.hpp"

namespace upsylon
{
    namespace Ink
    {
        Edges:: ~Edges() throw()
        {
        }

        Edges:: Edges( const size_t W, const size_t H ):
        pixels(W,H),
        grad_x(W,H),
        grad_y(W,H),
        grad(W,H),
        gmax(0),
        angle(W,H),
        border(W,H),
        compute_metrics_func( this , & Edges::compute_metrics_call),
        analyze_borders_func( this , & Edges::analyze_borders_call)
        {
        }

        void Edges:: compute_with(Engine &E)
        {
            // initialize
            E.acquire_all(sizeof(float));
            gmax=0;

            // start to run
            E.run(compute_metrics_func);
            gmax = E.get_max<float>();
            if(gmax>0)
            {
                E.run(analyze_borders_func);
            }
        }

        void Edges:: compute_metrics_call(const Tile &tile, lockable &)
        {
            Y_INK_AREA_LIMITS(tile);
            float local_gmax = 0;
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const PixmapF::Row &Gx = grad_x[y];
                const PixmapF::Row &Gy = grad_y[y];
                PixmapF::Row       &G  = grad[y];
                PixmapF::Row       &A  = angle[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const float gx = Gx[x];
                    const float gy = Gy[x];
                    const float g  = sqrt(gx*gx+gy*gy);
                    const float a  = math::__atan2(gy,gx);

                    G[x] = g;
                    A[x] = a;
                    if(g>local_gmax) local_gmax=g;
                }
            }
            tile.cache.get<float>() = local_gmax;
        }

        void Edges:: analyze_borders_call(const Tile &area, lockable &)
        {
            Y_INK_AREA_LIMITS(area);
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const PixmapF::Row &G  = grad[y];
                const PixmapF::Row &A  = angle[y];
                PixmapF::Row       &B  = border[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const float g0 = G[x];
                    float       gp = g0;
                    float       gn = g0;
                    {
                        const float  a  = A[x];
                        const unit_t dx = unit_t( floorf( cosf(a) + 0.5f ) );
                        const unit_t dy = unit_t( floorf( sinf(a) + 0.5f ) );
                        {
                            const coord prev(x-dx,y-dy);
                            if(grad.has(prev)) gp = grad[prev];
                        }

                        {
                            const coord next(x+dx,y+dy);
                            if(grad.has(next)) gn = grad[next];
                        }
                    }
                    if( (g0>=gp) && (g0>=gn) )
                    {
                        B[x] = g0/gmax;
                    }
                    else
                    {
                        B[x] = 0;
                    }
                }

            }
        }

    }

}

