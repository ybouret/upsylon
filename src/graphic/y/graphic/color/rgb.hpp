//! \file

#ifndef Y_GRAPHIC_COLOR_RGB_INCLUDED
#define Y_GRAPHIC_COLOR_RGB_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            template <typename T> class RGBA; //!< forward declaration

            //! RGB type
            template <typename T>
            class RGB
            {
            public:
                T r; //!< read
                T g; //!< green
                T b; //!< blue

                //! setup
                inline  RGB( T R=0, T G=0, T B=0) throw() : r(R), g(G), b(B) {}
                //! cleanup
                inline ~RGB() throw() {}
                //! copy
                inline  RGB(const RGB &_) throw() : r(_.r), g(_.g), b(_.b) {}
                //! assign
                inline  RGB & operator=(const RGB &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    return *this;
                }
                //! assign RGB part of RGBA
                RGB( const RGBA<T> &) throw();

            };

            //! RGBA
            template <typename T>
            class RGBA
            {
            public:
                static const T Opaque; //!< type dependent opaque value
                T r; //!< read
                T g; //!< green
                T b; //!< blue
                T a; //!< alpha

                //! setup
                inline  RGBA( T R=0, T G=0, T B=0, T A=Opaque) throw() : r(R), g(G), b(B), a(A) {}
                //! cleanup
                inline ~RGBA() throw() {}
                //! copy
                inline  RGBA(const RGBA &_) throw() : r(_.r), g(_.g), b(_.b), a(_.a) {}
                //! assign
                inline  RGBA & operator=(const RGBA &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    a=_.a;
                    return *this;
                }

                //! opaque RGB
                inline RGBA( const RGB<T> &C ) throw() : r(C.r), g(C.g), b(C.b), a(Opaque) {}


            };

            //! RGBA->RGB implementation
            template <typename T>
            inline RGB<T>::RGB( const RGBA<T> &C ) throw() : r(C.r), g(C.g), b(C.b) {}

        }

        typedef Kernel::RGB<uint8_t>  RGB;   //!< alias
        typedef Kernel::RGBA<uint8_t> RGBA;  //!< alias

    }
}

#endif


