//! \file
#ifndef Y_PLATFORM_INCLUDED
#define Y_PLATFORM_INCLUDED 1

//______________________________________________________________________________
//
//
// Platform definition
//
//______________________________________________________________________________
#if defined(Y_PLATFORM)
# undef Y_PLATFORM
#endif

#if defined(_WIN32)
#    define Y_PLATFORM "Windows"
#    define Y_WIN        1
#   if defined(_WIN32_WINNT)
#     undef  _WIN32_WINNT
#      define _WIN32_WINNT    0x0501
#   endif
#endif

#if defined(__MACH__) && ( defined(__APPLE__) || defined(__APPLE_CC__) )
#    define Y_PLATFORM    "Darwin"
#    define Y_BSD         1
#    define Y_APPLE       1
#endif

#if defined(__linux__)
#    define Y_PLATFORM    "Linux"
#    define Y_BSD          1
#    define Y_LINUX        1
#endif

#if defined(__FreeBSD__)
#    define Y_PLATFORM    "FreeBSD"
#    define Y_BSD          1
#    define Y_FREEBSD      1
#endif

#if defined(__OpenBSD__)
#    define Y_PLATFORM    "OpenBSD"
#    define Y_BSD          1
#    define Y_OPENBSD      1
#endif

#if defined(__sun__)
#    define Y_PLATFORM    "SunOS"
#    define Y_BSD         1
#    define Y_SUNOS       1
#endif

#if !defined(Y_PLATFORM)
# error "unhandled Y_PLATFORM"
#endif


//______________________________________________________________________________
//
//
// integer types
//
//______________________________________________________________________________
#if defined(Y_STDINT)
# undef Y_STDINT
#endif

//______________________________________________________________________________
//
// <GNUC>
//______________________________________________________________________________
#if defined(__GNUC__)
#define Y_STDINT 1
#include <stdint.h>
#    define    Y_U64(X) X##ULL
#    define    Y_I64(X) X##LL
#endif
//______________________________________________________________________________
//
// <GNUC/>
//______________________________________________________________________________


//______________________________________________________________________________
//
// <Microsoft>
//______________________________________________________________________________
#if defined(_MSC_VER)


#if _MSC_VER >= 1900
#include <stdint.h>
#else
// Microsoft built-in types
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
#endif

#        define    Y_STDINT 1
#        define    Y_U64(X) X##ui64
#        define    Y_I64(X) X##i64

#endif
//______________________________________________________________________________
//
// <Microsoft/>
//______________________________________________________________________________

#if !defined(Y_STDINT)
#    error "no STDINT"
#endif

#include <cstddef>
#include <cassert>

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

    //! default type selector.
    template <const bool flag, typename T, typename U> struct select;

    //! type selector when true => T
    template <typename T, typename U>
    struct select<true,T,U>
    {
        typedef T result; //!< flag is true
    };

    //! type selector when false => U
    template <typename T, typename U>
    struct select<false,T,U>
    {
        typedef U result; //!< flag is false
    };

    //! destructor disambiguation
    template <typename T>
    inline void __dtor( T *item ) throw()
    {
        assert(item);
        item->~T();
    }

    //! for verbose assert
    inline bool die( const char *) throw() { return false; }

    //! the signed system size
    typedef ptrdiff_t unit_t;
}

#endif

