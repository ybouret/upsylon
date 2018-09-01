//! \file
#ifndef Y_INK_TILES_INCLUDED
#define Y_INK_TILES_INCLUDED 1

#include "y/ink/area.hpp"
#include "y/memory/cslot.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! reusable local memory for one tile
        typedef memory::cslot<memory::global> LocalMemory;

        //! a tile is an area with a memory cache
        class Tile : public Area, public counted
        {
        public:
            typedef arc_ptr<Tile> Pointer;

            //! constructor
            inline Tile( const Area &a ) : Area(a), cache() {}

            //!destructor
            inline virtual ~Tile() throw() {}

            mutable LocalMemory cache;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
        };


        //! sequence of tiles base type
        typedef vector<Tile::Pointer> __Tiles;

        //! areas for tiling
        class Tiles : public __Tiles
        {
        public:
            //! destructor
            inline virtual ~Tiles() throw() {}

            //! copy
            //inline          Tiles(const Tiles &other) : dynamic(), __Tiles( other ) {}

            //! create tiles
            Tiles( const Area &source, const size_t n);
            

        private:
            Y_DISABLE_ASSIGN(Tiles);
        };

    }
}


#endif

