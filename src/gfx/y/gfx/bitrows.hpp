

//! \file

#ifndef Y_GFX_BITROWS_INCLUDED
#define Y_GFX_BITROWS_INCLUDED 1

#include "y/gfx/bitrow.hpp"
#include "y/gfx/pixels.hpp"

namespace upsylon
{
    namespace graphic
    {

        class bitrows : public entity
        {
        public:
            explicit bitrows(void        *entry,
                             const size_t width,
                             const size_t height,
                             const zflux &zhost,
                             const size_t shift);
            virtual ~bitrows() throw();

            crux::pixels impl;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitrows);
            size_t      done;
        };

    }

}

#endif
