
//! \file

#ifndef Y_GRAPHIC_PARALLEL_COPY_INCLUDED
#define Y_GRAPHIC_PARALLEL_COPY_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"

namespace upsylon {

    namespace Graphic {


        namespace Kernel {

            template <typename PIXMAP>
            struct Copy
            {
                const Tiles_ *tiles;
                const PIXMAP *source;
                PIXMAP       *target;

                static inline void Run(void *args, parallel &ctx, lockable &) throw()
                {
                    Copy &self = *static_cast<Copy *>(args);
                    
                }
            };

        }
    }
}

#endif
