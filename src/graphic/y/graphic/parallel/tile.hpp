
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILE_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILE_INCLUDED 1

#include "y/graphic/area.hpp"
#include "y/memory/xslot.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon {

    namespace Graphic {

        typedef memory::xslot<memory::pooled> TileCache; //!< alias

        //! subdivision for operation on bitmaps/pixmaps, may be empty
        class Tile : public Area, public TileCache
        {
        public:

            const Point  lower;        //!< lower index
            const Point  upper;        //!< upper index
            const size_t linearOffset; //!< how many items so far
            
            //! setup with possible zero Area
            explicit  Tile(const Point &lo, const Point &up);

            //! cleanup
            virtual  ~Tile() throw();

            //! display
            friend std::ostream & operator<<( std::ostream &, const Tile &);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
        };


    }

}

#endif

