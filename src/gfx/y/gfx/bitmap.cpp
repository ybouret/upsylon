
#include "y/gfx/bitmap.hpp"

namespace upsylon
{
    namespace GFX
    {
        Bitmap:: ~Bitmap() throw()
        {
        }

        static const char bfn[] = "Bitmap";

        Bitmap:: Bitmap(const unit_t W, const unit_t H, const size_t BPP) :
        w( Check::GTZ(W, Check::Width, bfn) ),
        h( Check::GTZ(H, Check::Width, bfn) ),
        bpp( Check::GTZ(BPP, "BPP", bfn)),
        scanline(w*bpp),
        stride(scanline),
        pixels( new Pixels_(h*stride) )
        {
        }


        std::ostream & operator<<(std::ostream &os, const Bitmap &bmp)
        {
            os << '[' << bmp.w << 'x' << bmp.h << 'x' << bmp.bpp << ' ' << bmp.pixels << ']';
            return os;
        }
    }
}


