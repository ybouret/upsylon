//! \file
#ifndef Y_MEMORY_IO_INCLUDED
#define Y_MEMORY_IO_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {
        //! operations on memory addresses
        struct io
        {

            //! casting with shift
            template <typename T>
            static T *cast( void *addr, const ptrdiff_t shift=0) throw()
            {
                char *p=static_cast<char *>(addr);
                void *q=static_cast<void *>(p+shift);
                return static_cast<T*>(q);
            }

            //! casting with shift, CONST version
            template <typename T>
            static const T *cast( const void *addr, const ptrdiff_t shift=0) throw()
            {
                const char *p = static_cast<const char *>(addr);
                const void *q = static_cast<const void *>(p+shift);
                return static_cast<T*>(q);
            }


            //! differential in bytes
            template <typename T>
            static ptrdiff_t delta(const T *a, const T *b) throw()
            {
                return static_cast<ptrdiff_t>(cast<const char>(b)-cast<const char>(a));
            }

            //! out of reach shift
            static void *       __shift( void *addr,       const ptrdiff_t bytes) throw();
            
            //! out of reach shift, CONST
            static const void * __shift( const void *addr, const ptrdiff_t bytes) throw();

            //! translation
            template <typename T> static inline
            T *__prev( T *addr ) throw()
            {
                static const ptrdiff_t shft = -ptrdiff_t(sizeof(T));
                return static_cast<T*>( __shift(addr,shft) );
            }
        };

    }
}

#endif

