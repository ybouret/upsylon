//! \file

#ifndef Y_GRAPHIC_COLOR_RGB_INCLUDED
#define Y_GRAPHIC_COLOR_RGB_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            template <typename T> class rgba; //!< forward declaration

            //! RGB type
            template <typename T>
            class rgb
            {
            public:
                T r; //!< read
                T g; //!< green
                T b; //!< blue

                //! setup
                inline  rgb( T R=0, T G=0, T B=0) throw() : r(R), g(G), b(B) {}
                //! cleanup
                inline ~rgb() throw() {}
                //! copy
                inline  rgb(const rgb &_) throw() : r(_.r), g(_.g), b(_.b) {}
                //! assign
                inline  rgb & operator=(const rgb &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    return *this;
                }
                //! assign RGB part of RGBA
                rgb( const rgba<T> &) throw();

                //! 0..2 access
                inline T & operator[](const size_t i) throw()
                {
                    assert(i<3);
                    return *((&r)+i);
                }

                //! 0..2 const access
                inline const T & operator[]( const size_t i) const throw()
                {
                    assert(i<3);
                    return *((&r)+i);
                }
                
            };

            //! RGBA
            template <typename T>
            class rgba
            {
            public:
                static const T Opaque; //!< type dependent opaque value
                T r; //!< read
                T g; //!< green
                T b; //!< blue
                T a; //!< alpha

                //! setup
                inline  rgba( T R=0, T G=0, T B=0, T A=Opaque) throw() : r(R), g(G), b(B), a(A) {}
                //! cleanup
                inline ~rgba() throw() {}
                //! copy
                inline  rgba(const rgba &_) throw() : r(_.r), g(_.g), b(_.b), a(_.a) {}
                //! assign
                inline  rgba & operator=(const rgba &_) throw()
                {
                    r=_.r;
                    g=_.g;
                    b=_.b;
                    a=_.a;
                    return *this;
                }

                //! opaque RGB
                inline rgba( const rgb<T> &C ) throw() : r(C.r), g(C.g), b(C.b), a(Opaque) {}

                //! 0..3 access
                inline T & operator[](const size_t i) throw()
                {
                    assert(i<4);
                    return *((&r)+i);
                }

                //! 0..3 const access
                inline const T & operator[]( const size_t i) const throw()
                {
                    assert(i<4);
                    return *((&r)+i);
                }

            };

            //! RGBA->RGB implementation
            template <typename T>
            inline rgb<T>::rgb( const rgba<T> &C ) throw() : r(C.r), g(C.g), b(C.b) {}

        }

        typedef Kernel::rgb<uint8_t>  rgb;   //!< alias
        typedef Kernel::rgba<uint8_t> rgba;  //!< alias

    }
}

#endif


