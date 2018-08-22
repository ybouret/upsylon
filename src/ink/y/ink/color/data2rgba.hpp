//! \file

#ifndef Y_INK_COLOR_DATA2RGBA_INCLUDED
#define Y_INK_COLOR_DATA2RGBA_INCLUDED 1

#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace ink
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

#define Y_INK_DATA2RGBA(NAME,CODE)                   \
class NAME : public data2rgba {                       \
public:                                                \
inline explicit NAME() throw() : data2rgba() {}         \
inline virtual ~NAME() throw() {}                        \
private:                                                  \
Y_DISABLE_COPY_AND_ASSIGN(NAME);                           \
inline virtual RGBA get(const void *addr) throw() { CODE; } \
}

        Y_INK_DATA2RGBA(rgba2rgba,  return *(RGBA *)addr);
        Y_INK_DATA2RGBA(rgb2rgba,   return RGBA(*(RGB *)addr));
        Y_INK_DATA2RGBA(float2rgba, const uint8_t u = Y_INK_F2B( *(float*)addr ); return RGBA(u,u,u));
        Y_INK_DATA2RGBA(byte2rgba,  const uint8_t u = *(uint8_t *)addr; return RGBA(u,u,u));

    }
}

#endif

