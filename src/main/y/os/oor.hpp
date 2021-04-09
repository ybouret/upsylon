//! \file

#ifndef Y_OS_OOR_INCLUDED
#define Y_OS_OOR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! out of reach wrappers
    struct out_of_reach
    {
        static void copy(void *target, const void *source, const size_t bytes) throw(); //!< memcpy
        static void move(void *target, const void *source, const size_t bytes) throw(); //!< memmove
        static void fill(void *target, const uint8_t byte, const size_t bytes) throw(); //!< memset
        
        static void fill_indices(size_t      *indices,
                                 const size_t length,
                                 const size_t offset) throw(); //!< indices[0..length-1] = [ offset...offset+length-1]

        static void       *shift(void       *, const ptrdiff_t) throw(); //!< shift address
        static const void *shift(const void *, const ptrdiff_t) throw(); //!< shift address

        //! get previous address
        template <typename T> static inline
        T * prev(T *obj) throw()
        {
            return static_cast<T*>(shift(obj,-static_cast<ptrdiff_t>(sizeof(T))));
        }


    };


}

#endif
