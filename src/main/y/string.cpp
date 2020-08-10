
#include "y/string.hpp"
#include "y/exceptions.hpp"
#include "y/memory/zblock.hpp"
#include "y/strfwd.hpp"
#include "y/memory/allocator/global.hpp"
#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace upsylon
{
    string vformat( const char *fmt,... )
    {
        typedef zblock<char,memory::global> zbuf;
        assert(fmt);
        size_t n = 32;
    TRY:
        {
            zbuf   databuf( n );
            char  *buffer  = *databuf;
            int    length  = int( databuf.length() );
            if( length < 0 )
                throw libc::exception( ERANGE, "string::vformat memory overflow");

            va_list ap;
            va_start( ap, fmt );
            const int  result  = vsnprintf( buffer, length, fmt, ap);
            const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
            va_end (ap);

            if( success )
            {
                return string( buffer, result );
            }
            n <<= 1;
        }
        goto TRY;

    }

    const as_string_t  as_string = {};

   
}


