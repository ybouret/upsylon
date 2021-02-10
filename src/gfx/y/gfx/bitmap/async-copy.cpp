
#include "y/gfx/bitmap.hpp"
#include "y/gfx/async/broker.hpp"


namespace upsylon
{
    namespace GFX
    {

        namespace
        {
            struct bitmapCopy
            {
                Bitmap       &target;
                const Bitmap &source;

                static inline void run(Async::Worker &w,
                                       lockable      &,
                                       void          *data)
                {
                    assert(data);
                    bitmapCopy   &self   = *static_cast<bitmapCopy*>(data);
                    const Tile   &tile   = w.tile;
                    Bitmap       &target = self.target;
                    const Bitmap &source = self.source;
                    const unit_t  depth  = source.depth;

                    for(size_t j=tile.size();j>0;--j)
                    {
                        const HScan  &hs  = tile[j];
                        memcpy( target.addressOf(hs.begin), source.addressOf(hs.begin), hs.width*depth );
                    }

                }
            };
        }

        Bitmap:: Bitmap(const Bitmap &bmp, Async::Broker &broker) :
        Area(bmp),
        depth(bmp.depth),
        scanline(bmp.scanline),
        stride(bmp.stride),
        pixels( new Pixels_(h*stride) ),
        rows(w,h,pixels->entry,stride)
        {
            bitmapCopy op = { *this, bmp };
            broker(op.run, &op);
        }

    }

}
