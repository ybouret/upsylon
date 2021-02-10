
//! \file

#ifndef Y_GFX_TILES_INCLUDED
#define Y_GFX_TILES_INCLUDED 1

#include "y/gfx/area/tile.hpp"

namespace upsylon
{
    namespace GFX
    {
        
        //______________________________________________________________________
        //
        //
        //! Tiles  
        //
        //______________________________________________________________________
        class Tiles : public Object, public Area
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Tiles() throw();            //!< cleanup
            static Tiles *Create(const Area  &area,
                                 const size_t maxThreads);

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
            const size_t            size;     //!< number of stretches
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            explicit Tiles(const Area   &area,
                           const size_t  maxThread); //!< setup
            size_t   count;
            size_t   bytes;
            Tile    *tile;
            
            void clear() throw();
        };

        typedef arc_ptr<const Tiles> Tiling;

    }

}

#endif


