//! \file
#ifndef Y_IPSO_TYPES_INCLUDED
#define Y_IPSO_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon
{
    namespace ipso
    {
        typedef unit_t           coord1D; //!< alias
        typedef point2d<coord1D> coord2D; //!< alias
        typedef point3d<coord1D> coord3D; //!< alias

        //! compute the dimensions of a coordinate type
        template <typename COORD>
        struct dim_of
        {
            static const size_t value = sizeof(COORD)/sizeof(coord1D); //!< DIMENSIONS
        };

        //! extract coordinate
        template <typename COORD>
        inline coord1D & coord_of( COORD &q, const size_t dim ) throw()
        {
            assert(dim<dim_of<COORD>::value);
            return *(((coord1D *)&q)+dim);
        }

        //! extract const coordinate
        template <typename COORD>
        inline const coord1D & coord_of( const COORD &q, const size_t dim ) throw()
        {
            assert(dim<dim_of<COORD>::value);
            return *(((const coord1D *)&q)+dim);
        }

    }
}

#endif

