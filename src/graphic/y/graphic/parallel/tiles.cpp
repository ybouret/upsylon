
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/parallel/tiling.hpp"

namespace upsylon {

    namespace Graphic {


        Tiles:: ~Tiles() throw()
        {
        }

        Tiles:: Tiles(const Area           &area,
                      concurrent::for_each &loop ) :
        Tiles_( loop.engine().num_threads() )
        {
            assert( count == loop.engine().num_threads() );

            const Cores  cores = Tiling::ComputeCoresFor(area,count);
            const size_t ncpus = cores.prod();

            for(size_t ry=0;ry<cores.y;++ry)
            {
                unit_t h = area.h;
                unit_t j = 0;
                parops::split_any(h, j, cores.y, ry);

                for(size_t rx=0;rx<cores.x;++rx)
                {
                    unit_t w = area.w;
                    unit_t i = 0;
                    parops::split_any(w, i, cores.x,rx);

                    const Point lower(i,j);
                    Point upper(i+w,j+h);
                    upper.dec();
                    this->build<const Point&,const Point&>(lower,upper);
                }

            }

            {
                const Point lo( 0, 0);
                const Point up(-1,-1);
                for(size_t n=ncpus;n<count;++n)
                {
                    this->build<const Point&,const Point&>(lo,up);
                }
            }
        }
    }
}
