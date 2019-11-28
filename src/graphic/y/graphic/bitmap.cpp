
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


        void Bitmap:: run( hashing::function &H ) const throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                H.run(rows[j].addr, scanline);
            }
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

            // check model
            switch(kind)
            {
                case Memory::Static: break;
                case Memory::Global:
                    *( nref = object::acquire1<size_t>() ) = 1;
                    break;
            }

            // create rows
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

        const void * AnonymousRow:: zfx(const unit_t i) const throw()
        {
            assert(addr);
            return static_cast<const char *>(addr) + bitmap.depth * bitmap.zfw(i);
        }

        const void * AnonymousRow:: std(const unit_t i) const throw()
        {
            assert(addr);
            assert(i>=0);
            assert(i<bitmap.w);
            return static_cast<const char *>(addr) + bitmap.depth * i;
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

        void Bitmap:: checkWritable(const char *context) const
        {
            assert(context);
            switch(mode)
            {
                case Memory::ReadWrite: break;
                case Memory::ReadOnly: throw exception("%sis Read-Only in <%s>!",fn,context);
                    break;
            }
        }

        void Bitmap:: clear()
        {
            checkWritable("clear");
            for(unit_t j=0;j<h;++j)
            {
                memset( rows[j].addr, 0, scanline );
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

        Bitmap  * Bitmap:: Clone(const Bitmap &bitmap, const Rectangle &rectangle)
        {
            if( ! bitmap.contains(rectangle) ) throw exception("%sinvalid sub-bitmap to clone",fn);
            const size_t BPP = bitmap.depth;
            Bitmap      *bmp = Create(rectangle.w,rectangle.h,BPP);

            const size_t n     = bmp->scanline;
            const size_t shift = rectangle.lower.x*BPP;
            const unit_t H     = bmp->h;
            for(unit_t j=0,k=rectangle.lower.y;j<H;++j,++k)
            {
                void        *target = bmp->rows[j].addr;
                const void  *source = static_cast<const char *>(bitmap.rows[k].addr) + shift;
                memcpy(target,source,n);
            }

            return bmp;
        }

        Bitmap  * Bitmap:: Clone(const Bitmap &bitmap )
        {
            return Clone(bitmap,bitmap);
        }

        const AnonymousRow *Bitmap:: zfxRow(const unit_t j) const throw()
        {
            return rows + zfh(j);
        }

        const AnonymousRow * Bitmap:: stdRow(const unit_t j) const throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows+j;
        }

        const void * Bitmap:: zfxGet(const unit_t i, const unit_t j) const throw()
        {
            return zfxRow(j)->zfx(i);
        }

        const void * Bitmap:: zfxGet(const Point &p) const throw()
        {
            return zfxGet(p.x,p.y);
        }

        const void * Bitmap:: stdGet(const unit_t i, const unit_t j) const throw()
        {
            return stdRow(j)->std(i);
        }

        const void * Bitmap:: stdGet(const Point &p) const throw()
        {
            return stdGet(p.x, p.y);
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

            if( ! bitmap.contains(rectangle) ) throw exception("%sinvalid sub-bitmap to share",fn);

            setupRows( (void*) ( bitmap.stdGet(rectangle.lower) ) );

            if(nref)
            {
                ++(*nref);
            }
        }

        Bitmap * Bitmap:: Share(const Bitmap &bitmap, const Rectangle &rectangle)
        {
            return new Bitmap(bitmap,rectangle);
        }

        Bitmap * Bitmap:: Share(const Bitmap &bitmap)
        {
            return new Bitmap(bitmap,bitmap);
        }


    }

}


