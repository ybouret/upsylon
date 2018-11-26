#ifndef Y_OS_SETUP_INCLUDED
#define Y_OS_SETUP_INCLUDED 1


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

#endif

