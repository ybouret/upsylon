

//! \file

#ifndef Y_GFX_AREA_TILES_INCLUDED
#define Y_GFX_AREA_TILES_INCLUDED 1


#include "y/gfx/area/tile.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/ref.hpp"

namespace upsylon
{
    namespace graphic
    {
        typedef const ref_ptr<const tile> pebble;

        //______________________________________________________________________
        //
        //
        //! tiles for any area
        //
        //______________________________________________________________________
        class tiles : public area, public slots<pebble,memory::dyadic>
        {
        public:
            //__________________________________________________________________
            //
            // types and defintions
            //__________________________________________________________________
            typedef slots<pebble, memory::dyadic > tiles_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            virtual ~tiles() throw();

            //! setup with required n => 1<=n<=a.items
            explicit tiles(const area  &a, const size_t n);

            //! setup for collection
            explicit tiles(const area &, const collection &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tiles);
            void initialize();
            static size_t size_for(const area &a, const size_t n) throw();
        };
    }

}

#endif

