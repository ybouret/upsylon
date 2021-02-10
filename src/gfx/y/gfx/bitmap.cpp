
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
        Area(0,0,Check::GTZ(W, Check::Width, bfn) ,Check::GTZ(H, Check::Width, bfn)),
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

        void Bitmap:: ldz() throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                memset(rows[j].p,0,scanline);
            }
        }

        void * Bitmap:: addressOf(const Point p) throw()
        {
            return rows[p.y](p.x,depth);
        }

        const void * Bitmap:: addressOf(const Point p) const throw()
        {
            return rows[p.y](p.x,depth);
        }


        Bitmap:: Bitmap(const Bitmap &bmp, const Area &area) :
        Area(0,0,Check::GTZ(area.w, Check::Width, bfn) ,Check::GTZ(area.h, Check::Width, bfn)),
        depth( bmp.depth ),
        scanline(w*depth),
        stride( bmp.stride ),
        pixels( bmp.pixels ),
        rows(w,h,bmp.rows[area.y](area.x,depth),stride)
        {
            if(!bmp.owns(area) ) throw exception("SubBitmap invalid area");
        }

        void *Bitmap:: oor_rows() throw()
        {
            return &rows[0];
        }

        
    }
}

#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace GFX
    {
        void Bitmap:: run(hashing::function &H) const throw()
        {
            for(unit_t j=0;j<h;++j)
            {
                H(rows[j].p,scanline);
            }
        }

        hashing::function & Bitmap:: hashWith(hashing::function&H) const throw()
        {
            H.set();
            run(H);
            return H;
        }

    }
}

