//! \file


#ifndef Y_TYPE_ALIASING_INCLUDED
#define Y_TYPE_ALIASING_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! function to overide casting limitations
    struct aliasing
    {
        //! unreachable address conversion
        static void *       __(void       *) throw();

        //! unreachable const address conversion
        static const void * __(const void *) throw();

        //! casting between same binary objects
        template <typename T,typename U> static inline
        T &cast( U &args ) throw()
        {
            return *static_cast<T*>(__(&args));
        }

        //! casting between same const binary objects
        template <typename T,typename U> static inline
        const T &cast( const U &args ) throw()
        {
            return *static_cast<const T*>(__(&args));
        }

        //! casting with overide between same binary objects
        template <typename T,typename U> static inline
        T &cast_( const U &args ) throw()
        {
            return cast<T,U>(_(args));
        }

        //! mapping a binary layout to object
        template <typename T,typename U> static inline
        T &map( U *arr ) throw()
        {
            assert(arr);
            return *static_cast<T*>( __(arr) );
        }

        //! mapping a const binary layout to object
        template <typename T,typename U> static inline
        const T &map(const U *arr ) throw()
        {
            assert(arr);
            return *static_cast<const T*>( __(arr) );
        }

        //! remove constness
        template <typename T> static inline
        T &_( const T &args ) throw()
        {
            return (T&)args;
        }

    };

}

#endif

