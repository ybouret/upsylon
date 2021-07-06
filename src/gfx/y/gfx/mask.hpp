
//! \file

#ifndef Y_GFX_MASK_INCLUDED
#define Y_GFX_MASK_INCLUDED 1

#include "y/gfx/types.hpp"
#include "y/associative/suffix/graph.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //! base type for mask
        //______________________________________________________________________
        typedef suffix_graph<uint8_t,coord> mask_;

        //______________________________________________________________________
        //
        //
        //! database of unique coordinates
        //
        //______________________________________________________________________
        class mask : public mask_
        {
        public:
            //! both big-endian coord for compact search
            static coord coord_to_key(const coord &) throw();

            mask();                     //!< setup empty
            mask(const mask&);          //!< copy
            virtual ~mask() throw();    //!< cleanup

            void insert(const coord p); //!< using coord_to_key

            mask & operator += (const coord); //!< add one coord
            mask & operator += (const mask&); //!< merge other mask

        private:
            Y_DISABLE_ASSIGN(mask);

        };

    }

}

#endif

