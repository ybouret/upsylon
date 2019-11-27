
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILES_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILES_INCLUDED 1

#include "y/graphic/parallel/tile.hpp"
#include "y/sequence/slots.hpp"
#include "y/concurrent/scheme/for-each.hpp"
#include "y/randomized/bits.hpp"

namespace upsylon {

    namespace Graphic {

        typedef slots<Tile> Tiles_;

        class Tiles : public Tiles_
        {
        public:
            explicit Tiles( const Area &area, concurrent::for_each &loop );
            virtual ~Tiles() throw();

            void randomize( randomized::bits &ran ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
        };

    }
}

#endif

