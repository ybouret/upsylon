
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/parallel/tiling.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Graphic {


        Tiles:: ~Tiles() throw()
        {
            localRelease();
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
        device(devs),
        dataBlock(0)
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


        void Tiles:: localRelease() throw()
        {
            Tiles_ &self = *this;
            dataBlock    = NULL;

            for(size_t i=0;i<count;++i)
            {
                self[i].data = 0;
                aliasing::_( self[i].size )  =0;
            }
        }

        void Tiles:: localAcquire(const size_t BytesPerTile)
        {
            if(BytesPerTile<=0)
            {
                return;
            }
            else
            {
                const size_t bytes = count * BytesPerTile;
                if( dataBlock.is_empty() || dataBlock->bytes<bytes )
                {
                    localRelease();
                    dataBlock = new Kernel::DataBlock(count,BytesPerTile);
                }
                assert(dataBlock->bytes>=bytes);
                Tiles_  &self = *this;
                uint8_t *p    = static_cast<uint8_t*>(dataBlock->entry);
                for(size_t i=0;i<count;++i,p+=BytesPerTile)
                {
                    self[i].data                = static_cast<void *>(p);
                    aliasing::_( self[i].size ) = BytesPerTile;
                }
            }
        }

    }
}

#include "y/code/round.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            DataBlock:: ~DataBlock() throw()
            {
                memory::global::location().release(entry,bytes);
            }

            DataBlock:: DataBlock(const size_t numTiles, const size_t BytesPerTile) :
            bytes( Y_ROUND4(BytesPerTile) * numTiles ),
            entry( memory::global::instance().acquire(bytes) )
            {
            }

        }

    }
}

