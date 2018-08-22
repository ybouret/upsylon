//! \file
#ifndef Y_INK_PIXEL_INCLUDED
#define Y_INK_PIXEL_INCLUDED 1

#include "y/ink/types.hpp"

namespace upsylon
{
    namespace ink
    {

        //! data for color types
        template <typename T>
        struct pixel
        {
            static const T zero;   //!< the zero value
            static const T opaque; //!< the opaque value
        };

    }
}

#if !defined(Y_INK_PIXEL_IMPL)

//! for external template field
#define _Y_PIXEL_DECL(TYPE,FIELD) extern template const TYPE upsylon::ink::pixel<TYPE>::FIELD

//! declare all external template fields
#define Y_PIXEL_DECL(T) \
_Y_PIXEL_DECL(T,zero);   \
_Y_PIXEL_DECL(T,opaque)

Y_PIXEL_DECL(float);     //!< for float pixel
Y_PIXEL_DECL(uint8_t);   //!< for byte pixel
Y_PIXEL_DECL(double);    //!< for double pixel
Y_PIXEL_DECL(uint16_t);  //!< for word pixel
Y_PIXEL_DECL(uint32_t);  //!< for dword pixel
Y_PIXEL_DECL(upsylon::ink::cplx); //!< for complex pixel

#endif

#endif

