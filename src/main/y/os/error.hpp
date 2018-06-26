
#ifndef Y_ERROR_INCLUDED
#define Y_ERROR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
#if defined(Y_BSD)
    typedef int      error_type;
#endif

#if defined(Y_WIN)
    typedef uint32_t error_type;
#endif

    namespace libc
    {
        void format_error( char buffer[], size_t length, int err ) throw();
        void critical_error( int err, const char when[] ) throw();
    }

    namespace win32
    {
        void format_error( char buffer[], size_t length, uint32_t err ) throw();
        void critical_error( uint32_t err, const char when[] ) throw();
    }

    void format_error( char buffer[], size_t length, error_type err ) throw();
    void critical_error( error_type err, const char when[] ) throw();
    extern const error_type error_invalid_data;
    inline void fatal_error(const char when[]) throw()
    {
        critical_error(error_invalid_data,when);
    }

}

#endif
