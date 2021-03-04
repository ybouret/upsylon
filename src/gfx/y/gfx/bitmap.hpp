
//! \file

#ifndef Y_GFX_BITMAP_INCLUDED
#define Y_GFX_BITMAP_INCLUDED 1

#include "y/gfx/bitrows.hpp"
#include "y/gfx/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {
        typedef arc_ptr<const crux::pixels> shared_pixels;

        class bitmap : public entity, public area
        {
        public:
            explicit bitmap(const unit_t W, const unit_t H, const unit_t D);
            virtual ~bitmap() throw();

            const size_t        depth;
            const size_t        stride;
            const shared_pixels pixels;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitmap);
        };

    }
    
}

#endif
