

//! \file

#ifndef Y_GFX_BITMAP_LINES_INCLUDED
#define Y_GFX_BITMAP_LINES_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace GFX
    {

        class PixRow
        {
        public:
            explicit PixRow(const void *p, const unit_t width) throw();
            virtual ~PixRow() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixRow);
        };

    }

}

#endif
