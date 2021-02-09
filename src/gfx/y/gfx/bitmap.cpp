
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
            _bzset(depth);
            _bzset(scanline);
            _bzset(stride);
        }

        static const char bfn[] = "Bitmap";

        Bitmap:: Bitmap(const unit_t W, const unit_t H, const size_t BPP) :
        w( Check::GTZ(W, Check::Width, bfn) ),
        h( Check::GTZ(H, Check::Width, bfn) ),
        depth( Check::GTZ(BPP, "depth", bfn)),
        scanline(w*depth),
        stride(scanline),
        pixels( new Pixels_(h*stride) ),
        rows(w,h,pixels->entry,stride)
        {
        }


        std::ostream & operator<<(std::ostream &os, const Bitmap &bmp)
        {
            os << '[' << bmp.w << 'x' << bmp.h << 'x' << bmp.depth << ' ' << bmp.pixels << ']';
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
            const Area b = bmp.area();
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
        depth( bmp.depth ),
        scanline(w*depth),
        stride( bmp.stride ),
        pixels( bmp.pixels ),
        rows(w,h,bmp.rows[area.y].at(area.x,depth),stride)
        {
        }

        void *Bitmap:: oor_rows() throw()
        {
            return &rows[0];
        }

        Area Bitmap:: area() const throw()
        {
            return Area(0,0,w,h);
        }

    }
}


