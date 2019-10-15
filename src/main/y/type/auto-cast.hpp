//! \file

#ifndef Y_TYPE_AUTO_CAST_INCLUDED
#define Y_TYPE_AUTO_CAST_INCLUDED 1

#include "y/type/class-conversion.hpp"

namespace upsylon {

    namespace core {

        //! type dependent conversion
        template <typename T, typename U, bool derived> struct auto_cast;

        //! conversion for derived type: bypass references
        template <typename T,typename U> struct auto_cast<T,U,true> {
            typedef T &       type;                                                 //!< mutable return type
            typedef const T & const_type;                                           //!< const   return type
            static inline type       from( U       &args ) throw() { return args; } //!< bypass
            static inline const_type from( const U &args ) throw() { return args; } //!< bypass
            };

        //! conversion for not dervied type: static_cast
        template <typename T,typename U> struct auto_cast<T,U,false> {
            typedef T type;                                                                 //!< mutable return type
            typedef T const_type;                                                           //!< const   return type
            static inline type       from( U       &args ) { return static_cast<T>(args); } //!< convert
            static inline const_type from( const U &args ) { return static_cast<T>(args); } //!< convert
        };

    }

    //! automatic argument casting
    template <typename T,typename U>
    struct auto_cast
    {
        static const bool                    derived = Y_IS_SUPERSUBCLASS(T,U); //!< check inheritance
        typedef core::auto_cast<T,U,derived> convert;                           //!< selected converter
        typedef typename convert::type       type;                              //!< alias
        typedef typename convert::const_type const_type;                        //!< alias

        static inline type       _( U       &args ) { return convert::from(args); } //!< argument conversion
        static inline const_type _( const U &args ) { return convert::from(args); } //!< argument const conversion

    };

}


#endif
