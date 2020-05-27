
//! \file

#ifndef Y_SPADE_VERTICES_INCLUDED
#define Y_SPADE_VERTICES_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {

    namespace Spade
    {
        //!   declaration for vertices, based on SPACE
        template <unsigned SPACE> struct VertexIn; //!< according to space
        
        //! Vertex in 1D
        template <> struct VertexIn<1>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef T Type; //!< the vertex type
            };
        };
        
        //! Vertex in 2D
        template <> struct VertexIn<2>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef point2d<T> Type; //!< the vertex type
            };
        };
        
        //! Vertex in 3D
        template <> struct VertexIn<3>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef point3d<T> Type; //!< the vertex type
            };
        };
        
        //!   declaration for vertices, based on COORD
        template <typename COORD> struct VertexFor
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef typename VertexIn<sizeof(COORD)/sizeof(Coord1D)>:: template Of<T>::Type Type; //!< alias
            };
        };


        
    }

}

#endif

