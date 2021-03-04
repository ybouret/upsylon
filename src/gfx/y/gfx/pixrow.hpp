
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

            inline type & operator()(const unit_t i) throw()
            {
                assert(i>=0); assert(i<w); return p[i];
            }

            inline const_type & operator()(const unit_t i) const throw()
            {
                assert(i>=0); assert(i<w); return p[i];
            }

            inline type & operator[](const unit_t i) throw()
            {
                return p[ z(i) ];
            }

            inline const_type & operator[](const unit_t i) const throw()
            {
                return p[ z(i) ];
            }


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
