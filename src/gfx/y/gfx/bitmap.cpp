
#include "y/gfx/bitmap.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"
#include <cstring>

namespace upsylon
{
    namespace GFX
    {
        Bitmap:: ~Bitmap() throw()
        {
            _bzset(w);
            _bzset(h);;
            _bzset(bpp);
            _bzset(scanline);
            _bzset(stride);
        }

        static const char bfn[] = "Bitmap";

        Bitmap:: Bitmap(const unit_t W, const unit_t H, const size_t BPP) :
        w( Check::GTZ(W, Check::Width, bfn) ),
        h( Check::GTZ(H, Check::Width, bfn) ),
        bpp( Check::GTZ(BPP, "BPP", bfn)),
        scanline(w*bpp),
        stride(scanline),
        pixels( new Pixels_(h*stride) ),
        rows(w,h,pixels->entry,stride)
        {
        }


        std::ostream & operator<<(std::ostream &os, const Bitmap &bmp)
        {
            os << '[' << bmp.w << 'x' << bmp.h << 'x' << bmp.bpp << ' ' << bmp.pixels << ']';
            return os;
        }

        static inline
        const Area &checkArea(const Bitmap &bmp, const Area &area)
        {
            static const char fn[] = "SubBitmap: ";
            if(area.n<=0)
            {
                throw exception("%sempty area",fn);
            }
            const Area b(0,0,bmp.w-1,bmp.h-1);
            if(!b.owns(area))
            {
                throw exception("%sinvalid area",fn);
            }
            return area;
        }

        void Bitmap:: ldz() throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                memset(rows[j].p,0,scanline);
            }
        }
        

        Bitmap:: Bitmap(const Bitmap &bmp, const Area &area) :
        w( checkArea(bmp,area).w ),
        h( area.h ),
        bpp( bmp.bpp ),
        scanline(w*bpp),
        stride( bmp.stride ),
        pixels( bmp.pixels ),
        rows(w,h,bmp.rows[area.y].at(area.x,bpp),stride)
        {
        }
    }
}


