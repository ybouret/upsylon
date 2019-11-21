
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"

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
                        Memory::Release(entry,bytes);
                    }
                    break;
            }
        }

        static const char fn[] = "Graphic::Bitmap: ";


        const Area & checkArea( const Area &area )
        {
            if(area.items<=0) throw exception("%sempty area",fn);
            return area;
        }

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

        Bitmap:: Bitmap(const Area        &area,
                        const void        *data,
                        const size_t       size,
                        const size_t       bytesPerPixel,
                        const size_t       dataStride,
                        const Memory::Kind memoryKind,
                        const Memory::Mode memoryMode) :
        Area(  checkArea(area) ),
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
            switch(kind)
            {
                case Memory::Static: break;
                case Memory::Global:
                    *( nref = object::acquire1<size_t>() ) = 1;
                    break;
            }
            try
            {
                setupRows();
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

        
        void Bitmap:: setupRows()
        {

            rlen = h;
            rows = Memory::AcquireAs<AnonymousRow>(rlen);
            AnonymousRow *r = rows;
            char         *p = (char *)entry;
            for(unit_t j=0;j<h;++j,p+=stride,++r)
            {
                new (r) AnonymousRow(p,*this);
            }

        }


        Bitmap * Bitmap:: Create(const size_t W,
                                 const size_t H,
                                 const size_t BPP)
        {
            if(W<=0||H<=0|BPP<=0) throw exception("%sinvalid %ux%u,depth=%u",fn, unsigned(W), unsigned(H), unsigned(BPP) );

            const Area area(W,H);
            size_t size = area.items*BPP;
            void  *data = Memory::Acquire(size);
            try {
                return new Bitmap(area,data,size,BPP,0,Memory::Global,Memory::ReadWrite);
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

        const void * Bitmap:: get(const unit_t i, const unit_t j) const throw()
        {
            return row(j)->pixel(i);
        }

        const void * Bitmap:: get(const Point &p) const throw()
        {
            return get(p.x,p.y);
        }

#if 0

        Bitmap:: Bitmap(const Bitmap    &bitmap,
                        const Rectangle &rectangle) :
        Area(rectangle),
        depth(bitmap.depth)
        {
        }

#endif
        

    }

}


