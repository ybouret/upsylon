

//! \file

#ifndef Y_GFX_BITROWS_INCLUDED
#define Y_GFX_BITROWS_INCLUDED 1

#include "y/gfx/pixels.hpp"
#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //! memory setup for rows
        //______________________________________________________________________
        class bitrows : public entity
        {
        public:
            //! build rows in impl memory
            explicit bitrows(void        *entry,
                             const size_t width,
                             const size_t height,
                             const size_t shift);
            //! cleanup
            virtual ~bitrows() throw();
            
            const   zflux zfw;  //!< shared zero flux/w
            crux::pixels  impl; //!< memory for this rows
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(bitrows);
            size_t      done;
        };

    }

}

#endif
