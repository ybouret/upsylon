
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/parallel/tiling.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

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


        ForEach_ & Tiles:: loop() throw()
        {
            return *device;
        }



        Tiles:: Tiles(const Area    &full,
                      const ForEach &devs ) :
        Tiles_( devs->number() ),
        area(full),
        device(devs)
        {
            assert( count == device->number() );

            const Cores  cores = Tiling::ComputeCoresFor(area,count);
            const size_t ncpus = cores.prod();
            size_t       linearOffset = 0;
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
                    Tile &tile = this->back();
                    aliasing::_( tile.linearOffset ) = linearOffset;
                    linearOffset += tile.items;
                }
            }
            assert( full.items == linearOffset );

            {
                const Point lo( 0, 0);
                const Point up(-1,-1);
                for(size_t n=ncpus;n<count;++n)
                {
                    this->build<const Point&,const Point&>(lo,up);
                }
            }
        }




        void Tiles:: cacheAcquire(const size_t BytesPerTile)
        {
            Tiles_ &self = *this;
            for(size_t i=0;i<count;++i)
            {
                self[i].acquire(BytesPerTile);
                assert( self[i].is_zeroed() );
            }
        }
        

    }
}


