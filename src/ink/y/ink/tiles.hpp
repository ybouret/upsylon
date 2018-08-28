//! \file
#ifndef Y_INK_TILES_INCLUDED
#define Y_INK_TILES_INCLUDED 1

#include "y/ink/area.hpp"
#include "y/memory/cslot.hpp"

namespace upsylon
{
    namespace ink
    {
        //! reusable local memory for one tile
        typedef memory::cslot<memory::global> local_memory;

        //! a tile is an area with a memory cache
        class tile : public area
        {
        public:
            //! constructor
            inline tile( const area &a ) : area(a), cache() {}

            //! copy
            inline tile(const tile &other) : area(other), cache() {}

            //!destructor
            inline virtual ~tile() throw() {}

            local_memory cache; //!< for parallel computation

        private:
            Y_DISABLE_ASSIGN(tile);
        };


        //! sequence of tiles base type
        typedef vector<tile> __tiles;

        //! areas for tiling
        class tiles : public __tiles
        {
        public:
            //! destructor
            inline virtual ~tiles() throw() {}

            //! copy
            inline          tiles(const tiles &other) : dynamic(), __tiles( other ) {}

            //! create tiles
            tiles( const area &source, const size_t n);


        private:
            Y_DISABLE_ASSIGN(tiles);
        };

    }
}


#endif

