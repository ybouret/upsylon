
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Graphic {

        Bitmap:: ~Bitmap() throw()
        {
            assert(rows);
            Memory::ReleaseAs(rows,rlen);
            switch(kind)
            {
                case Memory::Static: break;
                case Memory::Global:
                    assert(nref);
                    assert(*nref>0);
                    if( -- (*nref) <= 0 )
                    {
                        object::release1(nref);
                        Memory::Release(entry,aliasing::_(bytes));
                    }
                    break;
            }
        }

        static const char fn[] = "Graphic::Bitmap: ";


        static size_t checkDepth( const size_t bytesPerPixel )
        {

            if(bytesPerPixel<=0) throw exception("%sno bytesPerPixel",fn);
            return bytesPerPixel;
        }


        static size_t checkStride( size_t stride, const size_t scanline )
        {
            if(scanline<=0) throw exception("%sscanline<=0",fn);
            
            if( stride <= 0 )
            {
                return scanline;
            }
            else
            {
                if( stride<scanline ) throw exception("%sstride<scanline",fn);
                return stride;
            }
        }

        Bitmap:: Bitmap(const Rectangle   &rect,
                        const void        *data,
                        const size_t       size,
                        const size_t       bytesPerPixel,
                        const size_t       dataStride,
                        const Memory::Kind memoryKind,
                        const Memory::Mode memoryMode) :
        Rectangle(  rect ),
        depth( checkDepth(bytesPerPixel) ),
        scanline( w * depth ),
        stride( checkStride(dataStride, scanline) ),
        kind( memoryKind ),
        mode( memoryMode ),
        zfw(w),
        zfh(h),
        entry((void*)data),
        bytes(size),
        nref(0),
        rows(0),
        rlen(0)
        {
            // check enough memory
            if( stride*h > bytes ) throw exception("%snot enough data!!!",fn);

            switch(kind)
            {
                case Memory::Static: break;
                case Memory::Global:
                    *( nref = object::acquire1<size_t>() ) = 1;
                    break;
            }
            try
            {
                setupRows(entry);
            }
            catch(...)
            {
                if(nref) object::release1(nref);
                throw;
            }
        }

        AnonymousRow:: AnonymousRow(void              *address,
                                    const Bitmap      &parent) throw() :
        addr(address),
        bitmap(parent)
        {
        }

        const void * AnonymousRow:: pixel(const unit_t i) const throw()
        {
            assert(addr);
            return static_cast<const char *>(addr) + bitmap.depth * bitmap.zfw(i);
        }

        
        void Bitmap:: setupRows(void *origin)
        {
            assert(origin);
            rlen = h;
            rows = Memory::AcquireAs<AnonymousRow>(rlen);
            AnonymousRow *r = rows;
            char         *p = (char *)origin;
            for(unit_t j=0;j<h;++j,p+=stride,++r)
            {
                new (r) AnonymousRow(p,*this);
            }

        }


        Bitmap * Bitmap:: Create(const size_t W,
                                 const size_t H,
                                 const size_t BPP)
        {
            if( (W<=0) || (H<=0) || (BPP<=0) ) throw exception("%sinvalid %ux%u,depth=%u",fn, unsigned(W), unsigned(H), unsigned(BPP) );

            const Rectangle rect(0,0,W-1,H-1);
            size_t size = rect.items*BPP;
            void  *data = Memory::Acquire(size);
            try {
                return new Bitmap(rect,data,size,BPP,0,Memory::Global,Memory::ReadWrite);
            }
            catch(...)
            {
                Memory::Release(data,size);
                throw;
            }
        }


        const AnonymousRow *Bitmap:: row(const unit_t j) const throw()
        {
            return rows + zfh(j);
        }

        const AnonymousRow * Bitmap:: row__(const unit_t j) const throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows+h;
        }

        const void * Bitmap:: get(const unit_t i, const unit_t j) const throw()
        {
            return row(j)->pixel(i);
        }

        const void * Bitmap:: get(const Point &p) const throw()
        {
            return get(p.x,p.y);
        }

        Bitmap:: Bitmap(const Bitmap    &bitmap,
                        const Rectangle &rectangle) :
        Rectangle(0,0,rectangle.w-1,rectangle.h-1),
        depth(bitmap.depth),
        scanline(w*depth),
        stride(bitmap.stride),
        kind(bitmap.kind),
        mode(bitmap.mode),
        zfw(w),
        zfh(h),
        entry(bitmap.entry),
        bytes(bitmap.bytes),
        nref(bitmap.nref),
        rows(0),
        rlen(0)
        {

            if( ! bitmap.contains(rectangle) ) throw exception("%sinvalid sub-bitmap",fn);

            setupRows( (void*) ( bitmap.get(rectangle.lower) ) );

            if(nref)
            {
                ++(*nref);
            }
        }

        Bitmap * Bitmap:: Shared(const Bitmap &bitmap, const Rectangle &rectangle)
        {
            return new Bitmap(bitmap,rectangle);
        }


    }

}


