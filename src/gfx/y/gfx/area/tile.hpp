
//! \file

#ifndef Y_GFX_AREA_TILE_INCLUDED
#define Y_GFX_AREA_TILE_INCLUDED 1


#include "y/gfx/area.hpp"
#include "y/gfx/area/segment.hpp"
#include "y/memory/shack.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        // types for local memory
        //
        //______________________________________________________________________
        typedef memory::shack         local_memory; //!< alias
        typedef arc_ptr<local_memory> local_cache;  //!< alias

        //______________________________________________________________________
        //
        //
        //! a tile is a collection of contiguous segments
        //
        //______________________________________________________________________
        class tile :  public accessible<segment>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! get segments for size.rank from area (may be empty!)
            explicit tile(const area   &,
                          const size_t size,
                          const size_t rank);
            //! release memory
            virtual ~tile() throw();
           
            //__________________________________________________________________
            //
            // interface: accessible
            //__________________________________________________________________
            virtual size_t size() const throw();                             //!< lines
            virtual const segment & operator[](const size_t ) const throw(); //!< 1..lines
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            unit_t content() const throw(); //!< recompute content!
            coord  origin()  const throw(); //!< lower point
            area   aabb()    const throw(); //!< axis aligned bounding box

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t        lines; //!< number of lines
            mutable local_cache cache; //!< memory cache

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tile);
            segment *h_seg;
            size_t   count;
            size_t   bytes;
        public:
            const size_t        shift; //!< total items since origin

        };
    };
}

#endif

