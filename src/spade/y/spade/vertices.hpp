
//! \file

#ifndef Y_SPADE_VERTICES_INCLUDED
#define Y_SPADE_VERTICES_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {

    namespace Spade
    {
        //! forward declaration for fields instanciations
        template <typename COORD> struct VertexFor;

        //! Vertex1D of some sort
        template <> struct VertexFor<Coord1D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef T Type; //!< the vertex type
            };
        };

        //! Vertex2D of some sort
        template <> struct VertexFor<Coord2D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef point2d<T> Type; //!< the vertex type
            };
        };

        //! Vertex3D of some sort
        template <> struct VertexFor<Coord3D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef point3d<T> Type; //!< the vertex type
            };
        };

        
    }

}

#endif

