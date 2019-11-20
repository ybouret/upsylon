
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

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
        hh(h+h-2),
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
            rows   = memory::global::instance().acquire_as<AnonymousRow>(__rnum,__rlen);

            // link
            const unit_t ww = w+w-2;
            char  *p = (char *) entry;
            for(unit_t j=0;j<h;++j)
            {
                AnonymousRow &r  = rows[j];
                r.p     = p;
                aliasing::_(r.w)  = w;
                aliasing::_(r.ww) = ww;
                p += stride;
            }
        }

        const AnonymousRow * Bitmap:: row(const unit_t j) const throw()
        {
            if(j<0)
            {
                return row(-j);
            }
            else if(j>=h)
            {
                return row(hh-j);
            }
            else
            {
                return rows+j;
            }
        }

        const void *Bitmap:: get(const unit_t i, const unit_t j) const throw()
        {
            const AnonymousRow *r = row(j);
            const char         *p = static_cast<const char *>(r->p);
            return             &p[ r->indexOf(i) * depth ];
        }



        unit_t AnonymousRow::indexOf(const unit_t i) const throw()
        {
            if(i<0)
            {
                return indexOf(-i);
            }
            else if( i>= w )
            {
                return indexOf(ww-i);
            }
            else
            {
                return i;
            }
        }

        Bitmap::Bitmap( const Bitmap &bmp ) :
        Memory(bmp), Area(bmp),
        hh(bmp.hh),
        depth(bmp.depth),
        scanline(bmp.scanline),
        stride(bmp.stride),
        rows(0),
        __rnum(0),
        __rlen(0)
        {
            setup();
        }

        
    }
}
