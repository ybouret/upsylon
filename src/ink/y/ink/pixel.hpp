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

#define _Y_PIXEL_DECL(TYPE,FIELD) extern template const TYPE upsylon::ink::pixel<TYPE>::FIELD

#define Y_PIXEL_DECL(T) \
_Y_PIXEL_DECL(T,zero);   \
_Y_PIXEL_DECL(T,opaque)

Y_PIXEL_DECL(float);
Y_PIXEL_DECL(uint8_t);
Y_PIXEL_DECL(double);
Y_PIXEL_DECL(uint16_t);
Y_PIXEL_DECL(uint32_t);
Y_PIXEL_DECL(upsylon::ink::cplx);

#endif

#endif

