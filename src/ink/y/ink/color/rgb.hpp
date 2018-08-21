#ifndef Y_INK_COLOR_RGB_INCLUDED
#define Y_INK_COLOR_RGB_INCLUDED 1

#include "y/ink/color.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace ink
    {

        template <typename T>
        class rgb
        {
        public:
            Y_DECL_ARGS(T,type); //!< type alias
            type r; //!< red   channel
            type g; //!< green channel
            type b; //!< blue  channel

            inline  rgb( param_type R=0, param_type G=0, param_type B=0) throw() : r(R), g(G), b(B) {}
            inline ~rgb() throw() {}
            inline  rgb(const rgb &other) throw() : r(other.r), g(other.g), b(other.b) {}
            inline  rgb & operator=( const rgb &other) throw() { r=other.r; g=other.g; b=other.b; return *this; }

        };

        template <typename T>
        class rgba
        {
        public:
            Y_DECL_ARGS(T,type); //!< type alias
            type r; //!< red   channel
            type g; //!< green channel
            type b; //!< blue  channel
            type a; //!< alpha  channel

            inline  rgba( param_type R=0, param_type G=0, param_type B=0, param_type A=color<type>::opaque) throw() : r(R), g(G), b(B), a(A) {}
            inline ~rgba() throw() {}
            inline  rgba(const rgba   &other) throw() : r(other.r), g(other.g), b(other.b), a(other.a) {}
            inline  rgba(const rgb<T> &other,param_type A=color<type>::opaque) throw() : r(other.r), g(other.g), b(other.b), a(A) {}
            inline  rgba & operator=( const rgba   &other) throw() { r=other.r; g=other.g; b=other.b; a=other.a; return *this; }
            inline  rgba & operator=( const rgb<T> &other) throw() { r=other.r; g=other.g; b=other.b; return *this; }

        };

        typedef rgb<uint8_t>  RGB;
        typedef rgba<uint8_t> RGBA;

    }
}

#endif

