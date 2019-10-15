//! \file

#ifndef Y_TYPE_AUTO_CAST_INCLUDED
#define Y_TYPE_AUTO_CAST_INCLUDED 1

#include "y/type/class-conversion.hpp"

namespace upsylon {

    namespace core {

        template <typename T, typename U, bool derived> struct auto_cast;


        template <typename T,typename U> struct auto_cast<T,U,true> {
            typedef T &       type;
            typedef const T & const_type;
            static inline type       from( U       &args ) throw() { return args; }
            static inline const_type from( const U &args ) throw() { return args; }
            };

        template <typename T,typename U> struct auto_cast<T,U,false> {
            typedef T type;
            typedef T const_type;
            static inline type       from( U       &args ) { return static_cast<T>(args); }
            static inline const_type from( const U &args ) { return static_cast<T>(args); }
        };

    }

    template <typename T,typename U>
    struct auto_cast
    {
        static const bool                    derived = Y_IS_SUPERSUBCLASS(T,U);
        typedef core::auto_cast<T,U,derived> convert;
        typedef typename convert::type       type;
        typedef typename convert::const_type const_type;

        static inline type       _( U       &args ) { return convert::from(args); }
        static inline const_type _( const U &args ) { return convert::from(args); }

    };

}


#endif
