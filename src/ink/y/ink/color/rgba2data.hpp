//! \file
#ifndef Y_INK_COLOR_RGBA2DATA_INCLUDED
#define Y_INK_COLOR_RGBA2DATA_INCLUDED 1

#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! convert RGBA to something
        class rgba2data
        {
        public:
            //! destructor
            inline virtual ~rgba2data() throw() {}

            //! callable interface
            void operator()( void *addr, const RGBA &C ) throw()
            {
                assert(addr);
                put(addr,C);
            }

        protected:
            //! constructor
            inline explicit rgba2data() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rgba2data);
            virtual void put(void *addr,const RGBA &C) throw() = 0;
        };

        //! create derived class for rgba2data
#define Y_INK_RGBA2DATA(NAME,CODE)                            \
class NAME : public rgba2data {                                \
public:                                                         \
inline explicit NAME() throw() : rgba2data() {}                  \
inline virtual ~NAME() throw() {}                                 \
private:                                                           \
Y_DISABLE_COPY_AND_ASSIGN(NAME);                                    \
inline virtual void put(void *addr, const RGBA &C) throw() { CODE; } \
}
        //! RGBA -> RGBA
        Y_INK_RGBA2DATA(put_rgba, *(RGBA    *)addr = C);
        //! RGBA -> RGB
        Y_INK_RGBA2DATA(put_rgb,  *(RGB     *)addr = *(RGB *)&C);
        //! RGBA -> greyscale as float
        Y_INK_RGBA2DATA(put_gsf,  *(float   *)addr = Crux::GreyScaleF(C.r, C.g, C.b));
        //! RGBA -> greyscale as byte
        Y_INK_RGBA2DATA(put_gs1,  *(uint8_t *)addr = Crux::GreyScale1(C.r, C.g, C.b));

    }
}

#endif

