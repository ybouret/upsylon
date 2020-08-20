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

        
        //! U* -> T*
        template <typename T,typename U> static inline
        T *as(U *addr) throw()
        {
            void *p = static_cast<char *>( anonymous(addr) );
            return static_cast<T *>(p);
        }

        //! U* +/- shift -> T*
        template <typename T, typename U> static inline
        T *shifted(U *addr, const ptrdiff_t shift) throw()
        {
            return static_cast<T *>( anonymous(addr,shift) );
        }

        //! U* + offset -> T*
        template <typename T, typename U> static inline
        T *forward(U *addr, const size_t offset) throw()
        {
            void *p = static_cast<char *>(anonymous(addr)) + offset;
            return static_cast<T *>(p);
        }

        //! U* -> T* , const
        template <typename T,typename U> static inline
        const T *as(const U *addr) throw()
        {
            const void *p = static_cast<const char *>( anonymous(addr) );
            return static_cast<const T *>(p);
        }

        //! shifted in both directions
        template <typename T, typename U> static inline
        const T *shifted(const U *addr, const ptrdiff_t shift) throw()
        {
            return static_cast<const T *>( anonymous(addr,shift) );
        }

        //! forward only
        template <typename T, typename U> static inline
        const T *forward(const U *addr, const size_t offset) throw()
        {
            const void *p = static_cast<const char *>(anonymous(addr)) + offset;
            return static_cast<const T *>(p);
        }



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

        //! getting absolute bytes between object a->b
        template <typename T, typename U> static inline
        ptrdiff_t delta(const T *a, const U *b) throw()
        {
            return static_cast<ptrdiff_t>( as<const char>(b) - as<const char>(a) );
        }

        

    };

}

#endif

