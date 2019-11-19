
#include "y/graphic/bitmap.hpp"
#include "y/exception.hpp"

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
            setup();
        }


        void Bitmap:: setup()
        {
            const size_t span = stride * h;
            if(span>count) throw exception("Graphic::Bitmap(Invalid Memory)");
        }

    }
}
