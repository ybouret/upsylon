
//! \file

#ifndef Y_GFX_AREA_TILE_INCLUDED
#define Y_GFX_AREA_TILE_INCLUDED 1


#include "y/gfx/area.hpp"
#include "y/gfx/area/segment.hpp"

namespace upsylon
{
    namespace graphic
    {
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
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   lines; //!< number of lines
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tile);
            segment *h_seg;
            size_t   count;
            size_t   bytes;
        };
    };
}

#endif

