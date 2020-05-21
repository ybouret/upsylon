//! /file

#ifndef Y_SPADE_FIELDS_INCLUDED
#define Y_SPADE_FIELDS_INCLUDED 1

#include "y/spade/field/in3d.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {

    namespace Spade {

        //! forward declaration for fields instanciations
        template <typename COORD> struct FieldFor;

        //! Field1D of some sort
        template <> struct FieldFor<Coord1D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef Field1D<T> Type; //!< the field type
            };
        };

        //! Field2D of some sort
        template <> struct FieldFor<Coord2D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef Field2D<T> Type;//!< the field type
            };
        };

        //! Field3D of some sort
        template <> struct FieldFor<Coord3D>
        {
            //! compute type
            template <typename T> struct Of
            {
                typedef Field3D<T> Type;//!< the field type
            };
        };
        
        typedef intr_ptr<string,Field>   FieldHandle; //!< low-level field pointer
        typedef addressable<FieldHandle> Fields;
    }

}


#endif
