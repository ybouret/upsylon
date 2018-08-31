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
        angle(W,H),
        compute_metrics_func( this , & Edges::compute_metrics_call)
        {
        }

        void Edges:: compute_metrics_call(const Area &area, lockable &)
        {
            Y_INK_AREA_LIMITS(area);
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
                }
            }
        }

    }

}

