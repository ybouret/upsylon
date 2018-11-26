//! \file
#ifndef Y_PLATFORM_INCLUDED
#define Y_PLATFORM_INCLUDED 1

#include "y/os/setup.h"




namespace upsylon
{
    //! dummy platform function
    inline const char *platform() throw()
    {
        return Y_PLATFORM;
    }

    //! disable the copy constructor
#define Y_DISABLE_COPY(CLASS)            private: CLASS( const CLASS &)
    
    //! disable the assign operator
#define Y_DISABLE_ASSIGN(CLASS)          private: CLASS & operator=(const CLASS &)
    
    //! disable both copy constructor and assign operator
#define Y_DISABLE_COPY_AND_ASSIGN(CLASS) Y_DISABLE_COPY(CLASS); Y_DISABLE_ASSIGN(CLASS)
    
    //! integer to type mapping.
    template <int v>
    struct int2type {
        enum
        {
            value = v //!< a different class for each v.
        };
    };

    //! type to type mapping.
    template <class T>
    struct type2type
    {
        typedef T original_type; //!< a different light weight class for each T.
    };

    typedef type2type<size_t>   as_capacity_t; //!< for constructors
    extern const as_capacity_t  as_capacity;   //!< for constructors

    

    //! destructor disambiguation
    template <typename T>
    inline void destruct( T *item ) throw()
    {
        assert(item);
        item->~T();
    }

    //! for verbose assert
    inline bool die( const char *) throw() { return false; }

    //! the signed system size
    typedef ptrdiff_t unit_t;

    //! built-in CRC32
    uint32_t crc32(const void *buffer,const size_t buflen) throw();

    //! built-in integer hash
    uint32_t ih32(uint32_t a) throw();

}

#endif

