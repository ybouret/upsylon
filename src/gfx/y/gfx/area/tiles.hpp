
//! \file

#ifndef Y_GFX_TILES_INCLUDED
#define Y_GFX_TILES_INCLUDED 1

#include "y/gfx/area/topology.hpp"
#include "y/gfx/area/tile.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! Tiles of Stretches
        //
        //______________________________________________________________________
        class Tiles
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Tiles() throw(); //!< cleanup
            explicit Tiles(const size_t             cores,
                           const Topology::Pointer &topo); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! access stretch in 0<=rank<size
            const Tile & operator[](const size_t rank) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Topology::Pointer topology; //!< shared topopolgy
            const size_t            size;     //!< number of stretches
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            size_t   count;
            size_t   bytes;
            Tile    *tile;
            
            void clear() throw();
        };

    }

}

#endif


