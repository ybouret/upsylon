
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
            memory::global::location().release_as(rows,__rnum,__rlen);
        }

        void Bitmap:: setup()
        {
            // allocate rows
            assert(0==rows);
            assert(0==__rnum);
            assert(0==__rlen);
            __rnum = h;
            rows   = memory::global::instance().acquire_as<Row>(__rnum,__rlen);
            char  *p = (char *) entry;
            for(size_t j=0;j<h;++j)
            {
                Row &r  = rows[j];
                r.p     = p;
                r.w     = w;
                p += stride;
            }
        }

        const Bitmap::Row * Bitmap:: row(const size_t j) const throw()
        {
            if(j<0)
            {
                return 0;
            }
            else
            {
                return 0;
            }
        }
        
    }
}
