
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILE_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILE_INCLUDED 1

#include "y/graphic/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Graphic {


        //! subdivision for operation on bitmaps/pixmaps
        class Tile : public Area
        {
        public:

            typedef arc_ptr<Tile> Pointer;
            const Point lower;
            const Point upper;
            explicit  Tile(const Point &lo, const Point &up);
            virtual  ~Tile() throw();

            friend std::ostream & operator<<( std::ostream &, const Tile &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);

        };


    }

}

#endif

