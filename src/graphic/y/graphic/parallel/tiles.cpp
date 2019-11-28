
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/parallel/tiling.hpp"

namespace upsylon {

    namespace Graphic {


        Tiles:: ~Tiles() throw()
        {
        }

        void Tiles:: randomize(randomized::bits &ran) throw()
        {
            Tiles_ &self = *this;
            ran.shuffle(&self[0],count);
        }

        Tiles:: Tiles(const Area           &full,
                      const Device         &loop ) :
        Tiles_( loop->number() ),
        area(full),
        device(loop)
        {
            assert( count == device->number() );

            const Cores  cores = Tiling::ComputeCoresFor(area,count);
            const size_t ncpus = cores.prod();

            for(size_t ry=0;ry<cores.y;++ry)
            {
                unit_t height = area.h;
                unit_t j      = 0;
                parops::split_any(height, j, cores.y, ry);

                for(size_t rx=0;rx<cores.x;++rx)
                {
                    unit_t width = area.w;
                    unit_t i     = 0;
                    parops::split_any(width, i, cores.x,rx);

                    const Point lower(i,j);
                    Point upper(i+width,j+height);
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
