//! \file


#ifndef Y_TYPE_ALIASING_INCLUDED
#define Y_TYPE_ALIASING_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! function to overide casting limitations
    struct aliasing
    {
        //! unreachable address conversion
        static void *       anonymous(void *) throw();

        //! unreachable address conversion with shift
        static void *       anonymous(void *, const ptrdiff_t shift) throw();

        //! unreachable const address conversion
        static const void * anonymous(const void *) throw();

        //! unreachable const address conversion with shift
        static const void * anonymous(const void *, const ptrdiff_t shift) throw();

        //! casting between same binary objects
        template <typename T,typename U> static inline
        T &cast( U &args ) throw()
        {
            return *static_cast<T*>(anonymous(&args));
        }

        //! casting with overide between same const binary objects
        template <typename T,typename U> static inline
        const T &cast( const U &args ) throw()
        {
            return *static_cast<const T*>(anonymous(&args));
        }

        //! remove constness
        template <typename T> static inline
        T &_( const T &args ) throw()
        {
            return (T&)args;
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
            return *static_cast<T*>( anonymous(arr) );
        }

        //! mapping a const binary layout to object
        template <typename T,typename U> static inline
        const T &map(const U *arr ) throw()
        {
            assert(arr);
            return *static_cast<const T*>( anonymous(arr) );
        }


        template <typename T> static inline
        T *prev( T *obj ) throw()
        {
            return static_cast<T*>( anonymous(obj,-static_cast<ptrdiff_t>(sizeof(T))) );
        }

        template <typename T> static inline
        const T *prev( const T *obj ) throw()
        {
            return static_cast<T*>( anonymous(obj,-static_cast<ptrdiff_t>(sizeof(T))) );
        }

        template <typename T> static inline
        T *prev_( const T *obj ) throw()
        {
            return prev( _(obj) );
        }


    };

}

#endif

