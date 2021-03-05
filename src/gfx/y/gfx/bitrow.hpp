//! \file

#ifndef Y_GFX_BITROW_INCLUDED
#define Y_GFX_BITROW_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //! ABI compatible row for bitmap/pixmap
        //______________________________________________________________________
        class bitrow
        {
        public:
            //! setup
            bitrow(void        *entry,
                   const unit_t width,
                   const zflux &zhost) throw();
            ~bitrow() throw(); //!< cleanup
            
            void *       at(const unit_t i, const unit_t depth)       throw(); //!< raw access
            const void * at(const unit_t i, const unit_t depth) const throw(); //!< raw access, const

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitrow);
        public:
            void        *p; //!< address
            const unit_t w; //!< width
            const zflux &z; //!< zero flux for width
        };
        

    }

}

#endif
