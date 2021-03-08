

//! \file

#ifndef Y_GFX_AREA_TILES_INCLUDED
#define Y_GFX_AREA_TILES_INCLUDED 1


#include "y/gfx/area/tile.hpp"
#include "y/ptr/arc.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! tiles for any area
        //
        //______________________________________________________________________
        class tiles : public area, public slots< arc_ptr<tile>, memory::dyadic >
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            typedef slots< arc_ptr<tile>, memory::dyadic > tiles_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            virtual ~tiles() throw();

            //! setup with required n => 1<=n<=a.items
            explicit tiles(const area  &a,
                           const size_t n);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tiles);
        };
    }

}

#endif

