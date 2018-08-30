//! \file

#ifndef Y_INK_COLOR_DATA2RGBA_INCLUDED
#define Y_INK_COLOR_DATA2RGBA_INCLUDED 1

#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! convert data from an address into RGBA
        class data2rgba
        {
        public:
            //! destructor
            inline virtual ~data2rgba() throw() {}

            //! return a RGBA value from an address
            inline RGBA operator()(const void *addr) throw() { assert(addr); return get(addr); }


        protected:
            //! constructor
            inline explicit data2rgba() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data2rgba);
            virtual RGBA get(const void *addr) throw() = 0;
        };

        //! create derived class from data2rgba
#define Y_INK_DATA2RGBA(NAME,CODE)                   \
class NAME : public data2rgba {                       \
public:                                                \
inline explicit NAME() throw() : data2rgba() {}         \
inline virtual ~NAME() throw() {}                        \
private:                                                  \
Y_DISABLE_COPY_AND_ASSIGN(NAME);                           \
inline virtual RGBA get(const void *addr) throw() { CODE; } \
}

        //! RGBA -> RGBA
        Y_INK_DATA2RGBA(get_from_rgba,   return *(RGBA *)addr);
        //! RGB  -> RGBA
        Y_INK_DATA2RGBA(get_from_rgb,    return RGBA(*(RGB *)addr));
        //! float -> RGBA
        Y_INK_DATA2RGBA(get_from_float,  const uint8_t u = Y_INK_F2B( *(float*)addr ); return RGBA(u,u,u));
        //! byte  -> RGBA
        Y_INK_DATA2RGBA(get_from_byte,   const uint8_t u = *(uint8_t *)addr; return RGBA(u,u,u));

    }
}

#endif

