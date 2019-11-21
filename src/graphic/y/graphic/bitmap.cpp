
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        Bitmap:: ~Bitmap() throw()
        {
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
        entry((void*)data),
        bytes(size),
        nref(0)
        {
            switch(kind)
            {
                case Memory::Static: break;
                case Memory::Global:
                    *( nref = object::acquire1<size_t>() ) = 1;
                    break;
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

    }

}


