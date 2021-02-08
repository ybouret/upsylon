
//! \file

#ifndef Y_GFX_TILES_INCLUDED
#define Y_GFX_TILES_INCLUDED 1

#include "y/gfx/topology.hpp"
#include "y/gfx/stretch.hpp"
#include "y/sequence/slots.hpp"

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
            virtual ~Tiles() throw(); //!< cleanup
            explicit Tiles(const size_t          cores,
                           const Topology::Pointer &topo); //!< setup

            //! access stretch in 0<=rank<size
            const Stretch & operator[](const size_t rank) const throw();

            const Topology::Pointer topology; //!< shared topopolgy
            const size_t            size;     //!< number of stretches
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            size_t   count;
            size_t   bytes;
            Stretch *stretch;
            void clear() throw();
        };

    }

}

#endif


