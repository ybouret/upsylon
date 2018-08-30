//! \file
#ifndef Y_INK_PIXEL_INCLUDED
#define Y_INK_PIXEL_INCLUDED 1

#include "y/ink/types.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! data for color types
        template <typename T>
        struct Pixel
        {
            static const T Zero;   //!< the zero value
            static const T Opaque; //!< the opaque value
        };

    }
}

#if !defined(Y_INK_PIXEL_IMPL)

//! for external template field
#define _Y_PIXEL_DECL(TYPE,FIELD) extern template const TYPE upsylon::Ink::Pixel<TYPE>::FIELD

//! declare all external template fields
#define Y_PIXEL_DECL(T) \
_Y_PIXEL_DECL(T,Zero);   \
_Y_PIXEL_DECL(T,Opaque)

Y_PIXEL_DECL(float);     //!< for float pixel
Y_PIXEL_DECL(uint8_t);   //!< for byte pixel
Y_PIXEL_DECL(double);    //!< for double pixel
Y_PIXEL_DECL(uint16_t);  //!< for word pixel
Y_PIXEL_DECL(uint32_t);  //!< for dword pixel
Y_PIXEL_DECL(upsylon::Ink::cplx); //!< for complex pixel

#endif

#endif

