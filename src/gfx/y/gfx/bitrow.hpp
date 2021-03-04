//! \file

#ifndef Y_GFX_BITROW_INCLUDED
#define Y_GFX_BITROW_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace gfx
    {

        //! ABI compatible row for bitmap/pixmpa
        class bitrow
        {
        public:
            bitrow(void        *entry,
                   const size_t width,
                   const zflux &zhost) throw();
            ~bitrow() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitrow);
            void *       p;
        public:
            const size_t w;
            const zflux &z;
        };
        

    }

}

#endif
