
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILES_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILES_INCLUDED 1

#include "y/graphic/parallel/tile.hpp"
#include "y/sequence/slots.hpp"
#include "y/concurrent/scheme/for-each.hpp"
#include "y/randomized/bits.hpp"

namespace upsylon {

    namespace Graphic {


        typedef slots<Tile>                   Tiles_;  //!< base class for Tiles
        typedef concurrent::for_each          ForEach_;
        typedef concurrent::for_each::pointer ForEach;

        //! Tiles to partition an area for parallel tasks
        class Tiles : public Tiles_
        {
        public:
            const Area   area; //!< original area

            //! setup for a given area and and a level of parallelism
            explicit Tiles(const Area    &full,
                           const ForEach &devs );

            //! cleanup
            virtual ~Tiles() throw();

            //! randomize to change CPU addressing
            void randomize( randomized::bits &ran ) throw();

            ForEach_ & loop() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            ForEach device;

        };

    }
}

#endif

