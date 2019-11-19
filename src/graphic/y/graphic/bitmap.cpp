
#include "y/graphic/bitmap.hpp"


namespace upsylon {

    namespace Graphic {

        Bitmap:: ~Bitmap() throw()
        {
        }

        Bitmap:: Bitmap(const size_t W,
                        const size_t H,
                        const size_t D,
                        const Mode   m) :
        Metrics(W,H,D),
        Memory(items*depth,m)
        {
        }


    }
}
