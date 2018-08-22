//! \file
#ifndef Y_INK_COLOR_RGB_INCLUDED
#define Y_INK_COLOR_RGB_INCLUDED 1

#include "y/ink/pixel.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace ink
    {

        //! packed r,g,b values
        template <typename T>
        class rgb
        {
        public:
            Y_DECL_ARGS(T,type); //!< type alias
            type r; //!< red   channel
            type g; //!< green channel
            type b; //!< blue  channel

            //! constructor
            inline  rgb( param_type R=0, param_type G=0, param_type B=0) throw() : r(R), g(G), b(B) {}
            //! destructor
            inline ~rgb() throw() {}
            //! copy
            inline  rgb(const rgb &other) throw() : r(other.r), g(other.g), b(other.b) {}
            //! assign
            inline  rgb & operator=( const rgb &other) throw() { r=other.r; g=other.g; b=other.b; return *this; }

        };

        //! packed r,g,b,a value
        template <typename T>
        class rgba
        {
        public:
            Y_DECL_ARGS(T,type); //!< type alias
            type r; //!< red   channel
            type g; //!< green channel
            type b; //!< blue  channel
            type a; //!< alpha  channel

            //! constructor
            inline  rgba( param_type R=0, param_type G=0, param_type B=0, param_type A=pixel<type>::opaque) throw() : r(R), g(G), b(B), a(A) {}
            //! destructor
            inline ~rgba() throw() {}
            //! copy
            inline  rgba(const rgba   &other) throw() : r(other.r), g(other.g), b(other.b), a(other.a) {}
            //! copy with different alpha channel
            inline  rgba(const rgb<T> &other,param_type A=pixel<type>::opaque) throw() : r(other.r), g(other.g), b(other.b), a(A) {}
            //! assign
            inline  rgba & operator=( const rgba   &other) throw() { r=other.r; g=other.g; b=other.b; a=other.a; return *this; }
            //! assign only rgb, keep alpha channel
            inline  rgba & operator=( const rgb<T> &other) throw() { r=other.r; g=other.g; b=other.b; return *this; }

        };

        typedef rgb<uint8_t>  RGB;   //!< default 3-bytes RGB
        typedef rgba<uint8_t> RGBA;  //!< default 4-bytes RGBA

    }
}

#if !defined(Y_INK_PIXEL_IMPL)
Y_PIXEL_DECL(upsylon::ink::RGB);  //!< external template
Y_PIXEL_DECL(upsylon::ink::RGBA); //!< external template
#endif


#endif

