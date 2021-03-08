
//! \file

#ifndef Y_GFX_AREA_TESSELLATION_INCLUDED
#define Y_GFX_AREA_TESSELLATION_INCLUDED 1


#include "y/gfx/area/tile.hpp"
#include "y/sequence/slots.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! tesselation of any area
        //
        //______________________________________________________________________
        class tessellation :
        public entity,
        public area,
        public slots< arc_ptr<tile>, memory::dyadic >
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
            virtual ~tessellation() throw();
            
            //! setup with required n => 1<=n<=a.items
            explicit tessellation(const area  &a,
                                  const size_t n);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tessellation);
        };

        //! dynamic tessellation => tiles
        typedef arc_ptr<tessellation> tiles;
    }
    
}

#endif
