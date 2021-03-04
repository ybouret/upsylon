

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
                             const size_t count,
                             const size_t width,
                             const zflux &zhost,
                             const size_t shift);
            virtual ~bitrows() throw();

            pixels impl;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitrows);
            size_t done;
        };

        typedef arc_ptr<bitrows> shared_rows;
    }

}

#endif
