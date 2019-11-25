//! \file

#ifndef Y_GRAPHIC_COLOR_RGB_INCLUDED
#define Y_GRAPHIC_COLOR_RGB_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            template <typename T> class RGBA;

            template <typename T>
            class RGB
            {
            public:
                T r;
                T g;
                T b;

                inline  RGB( T R=0, T G=0, T B=0) throw() : r(R), g(G), b(B) {}
                inline ~RGB() throw() {}
                inline  RGB(const RGB &_) throw() : r(_.r), g(_.g), b(_.b) {}
                inline  RGB & operator=(const RGB &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    return *this;
                }

                RGB( const RGBA<T> &) throw();

            };

            template <typename T>
            class RGBA
            {
            public:
                static const T Opaque;
                T r;
                T g;
                T b;
                T a;

                inline  RGBA( T R=0, T G=0, T B=0, T A=Opaque) throw() : r(R), g(G), b(B), a(A) {}
                inline ~RGBA() throw() {}
                inline  RGBA(const RGBA &_) throw() : r(_.r), g(_.g), b(_.b), a(_.a) {}
                inline  RGBA & operator=(const RGBA &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    a=_.a;
                    return *this;
                }

                inline RGBA( const RGB<T> &C ) throw() : r(C.r), g(C.g), b(C.b), a(Opaque) {}


            };

            template <typename T>
            inline RGB<T>::RGB( const RGBA<T> &C ) throw() : r(C.r), g(C.g), b(C.b) {}

        }

        typedef Kernel::RGB<uint8_t>  RGB;
        typedef Kernel::RGBA<uint8_t> RGBA;

    }
}

#endif


