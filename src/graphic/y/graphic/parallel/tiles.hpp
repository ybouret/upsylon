
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILES_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILES_INCLUDED 1

#include "y/graphic/parallel/tile.hpp"
#include "y/sequence/slots.hpp"
#include "y/concurrent/scheme/for-each.hpp"
#include "y/randomized/bits.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Graphic {

        typedef slots<Tile>                   Tiles_;  //!< base class for Tiles
        typedef concurrent::for_each          ForEach_;
        typedef concurrent::for_each::pointer ForEach;

        namespace Kernel {

            class DataBlock : public Object
            {
            public:
                explicit DataBlock( const size_t numTiles, const size_t BytesPerTile );
                virtual ~DataBlock() throw();
                size_t  bytes;
                void   *entry;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(DataBlock);
            };
        }


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

            //! get internal loop
            ForEach_ & loop() throw();

            void localAcquire(const size_t BytesPerTile);
            void localRelease() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            ForEach                     device;
            auto_ptr<Kernel::DataBlock> dataBlock;
        };

    }
}

#endif

