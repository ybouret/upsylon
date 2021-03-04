
//! \file

#ifndef Y_GFX_PIXROW_INCLUDED
#define Y_GFX_PIXROW_INCLUDED 1

#include "y/gfx/bitrow.hpp"

namespace upsylon
{
    namespace graphic
    {
        template <typename T>
        class pixrow
        {
        public:
            Y_DECL_ARGS(T,type);
        private:
            pixrow() throw(); ~pixrow() throw();
            Y_DISABLE_COPY_AND_ASSIGN(pixrow);
            mutable_type *p;
        public:
            const unit_t  w;
            const zflux  &z;
        };
    }

}

#endif
