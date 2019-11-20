
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        static const char fn[] = "Graphic::Bitmap: ";

        static inline size_t checkDepth( const size_t depth )
        {
            if(depth<=0)
            {
                throw exception("%sdepth=0",fn);
            }
            return depth;
        }

        Bitmap:: Bitmap( Metrics &metrics ) :
        Memory(metrics),
        Area(metrics.width,metrics.height),
        depth( checkDepth(metrics.depth) ),
        scanline(w*depth),
        stride(metrics.stride),
        rows(0),
        __rnum(0),
        __rlen(0)
        {
            if(stride<scanline) throw exception("%sstride<scanline",fn);
            setup();
        }

        Bitmap:: ~Bitmap() throw()
        {
        }

        void Bitmap:: setup()
        {
        }
        
    }
}
