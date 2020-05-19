//! /file

#ifndef Y_SPADE_FIELDS_INCLUDED
#define Y_SPADE_FIELDS_INCLUDED 1

#include "y/spade/field/in3d.hpp"

namespace upsylon {

    namespace Spade {


        template <typename COORD> struct FieldFor;

        template <> struct FieldFor<Coord1D>
        {
            template <typename T> struct Of
            {
                typedef Field1D<T> Type;
            };
        };

        template <> struct FieldFor<Coord2D>
        {
            template <typename T> struct Of
            {
                typedef Field2D<T> Type;
            };
        };

        template <> struct FieldFor<Coord3D>
        {
            template <typename T> struct Of
            {
                typedef Field3D<T> Type;
            };
        };
        

    }

}


#endif
