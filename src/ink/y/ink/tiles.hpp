//! \file
#ifndef Y_INK_TILES_INCLUDED
#define Y_INK_TILES_INCLUDED 1

#include "y/ink/area.hpp"
#include "y/memory/cslot.hpp"

namespace upsylon
{
    namespace ink
    {
        typedef memory::cslot<memory::global> local_memory;

        class tile : public area
        {
        public:
            inline tile( const area &a ) : area(a), cache() {}
            inline tile(const tile &other) : area(other), cache() {}
            inline virtual ~tile() throw() {}

            local_memory cache;

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

