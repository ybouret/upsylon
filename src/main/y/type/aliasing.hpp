//! \file


#ifndef Y_TYPE_ALIASING_INCLUDED
#define Y_TYPE_ALIASING_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {


    struct aliasing
    {
        static void *       __(void       *) throw();
        static const void * __(const void *) throw();

        template <typename T,typename U> static inline
        T &cast( U &args ) throw()
        {
            return *static_cast<T*>(__(&args));
        }

        template <typename T,typename U> static inline
        const T &cast( const U &args ) throw()
        {
            return *static_cast<const T*>(__(&args));
        }

        template <typename T,typename U> static inline
        T &map( U *arr ) throw()
        {
            assert(arr);
            return *static_cast<T*>( __(arr) );
        }

        template <typename T,typename U> static inline
        const T &map(const U *arr ) throw()
        {
            assert(arr);
            return *static_cast<const T*>( __(arr) );
        }
        

    };

}

#endif

