//! \file


#ifndef Y_TYPE_ALIASING_INCLUDED
#define Y_TYPE_ALIASING_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! functions to overide casting limitations
    //
    //__________________________________________________________________________
    struct aliasing
    {
        //! remove constness
        template <typename T> static inline
        T &_( const T &args ) throw() { return (T&)args; }


        //______________________________________________________________________
        //
        // out of reach functions
        //______________________________________________________________________
        //! unreachable address conversion
        static void *       anonymous(void *) throw();

        //! unreachable address conversion with shift in bytes
        static void *       anonymous(void *, const ptrdiff_t shift) throw();

        //! unreachable const address conversion
        static const void * anonymous(const void *) throw();

        //! unreachable const address conversion with shift in bytes
        static const void * anonymous(const void *, const ptrdiff_t shift) throw();

        //______________________________________________________________________
        //
        //! casting with same binary layout objects
        //______________________________________________________________________
        struct cast
        {
            //! U -> T
            template <typename T,typename U> static inline
            T & to(U &args) throw()
            {
                return *static_cast<T*>(anonymous(&args));
            }

            //! U -> T
            template <typename T,typename U> static inline
            const T & to(const U &args) throw()
            {
                return *static_cast<const T*>(anonymous(&args));
            }

        };


        //______________________________________________________________________
        //
        //! mapping a binary layout to object
        //______________________________________________________________________
        struct map
        {
            //! U* -> T
            template <typename T,typename U> static inline
            T &to( U *arr ) throw()
            {
                assert(arr);
                return *static_cast<T*>( anonymous(arr) );
            }

            //! U* -> T
            template <typename T,typename U> static inline
            const T &to(const U *arr ) throw()
            {
                assert(arr);
                return *static_cast<const T*>( anonymous(arr) );
            }
        };


        //______________________________________________________________________
        //
        // helpers for out of reach previous object
        //______________________________________________________________________

        //! get previous object
        template <typename T> static inline
        T *prev( T *obj ) throw()
        {
            return static_cast<T*>( anonymous(obj,-static_cast<ptrdiff_t>(sizeof(T))) );
        }


        //! get previous const object
        template <typename T> static inline
        const T *prev( const T *obj ) throw()
        {
            return static_cast<T*>( anonymous(obj,-static_cast<ptrdiff_t>(sizeof(T))) );
        }

        //______________________________________________________________________
        //
        // helpers
        //_____________________________________________________________________

        //! getting absolut bytes between object a->b
        template <typename T, typename U> static inline
        ptrdiff_t delta(const T *a, const T *b) throw()
        {
            return static_cast<ptrdiff_t>( (&map::to<const char>(b)) - (&map::to<const char>(a)) );
        }

        

    };

}

#endif

