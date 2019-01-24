
#include "y/ink/ops/crypt.hpp"

namespace upsylon
{
    namespace Ink
    {

        void Crypt::Run(Bitmap &tgt, const Bitmap &src, crypto::kstream &ks) throw()
        {
            assert(tgt.h==src.h);
            assert(tgt.w==src.w);
            assert(tgt.depth==src.depth);

            const size_t h = tgt.h;
            const size_t n = tgt.scanline;
            for(size_t j=0;j<h;++j)
            {
                const uint8_t *s = static_cast<const uint8_t *>( src.get_line(j) );
                uint8_t       *t = static_cast<uint8_t       *>( tgt.get_line(j) );
                for(size_t i=n;i>0;--i)
                {
                    *(t++) = ks( *(s++) );
                }
            }
        }
    }
}

