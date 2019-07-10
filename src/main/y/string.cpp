
#include "y/string.hpp"
#include "y/exceptions.hpp"
#include "y/memory/buffers.hpp"

#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace upsylon
{
    string vformat( const char *fmt,... )
    {
        assert(fmt);
        size_t n = 32;
    TRY:
        {
            memory::buffer_of<char,memory::global> databuf( n );
            char  *buffer  = *databuf;
            int    length  = int( databuf.size );
            if( length < 0 )
                throw libc::exception( ERANGE, "string::vformat memory overflow");

            va_list ap;
            va_start( ap, fmt );
            const int  result  = vsnprintf( buffer, length, fmt, ap);
            const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
            va_end (ap);

            if( success )
            {
                return string( buffer );
            }
            n <<= 1;
        }
        goto TRY;

    }


    std::ostream & display_align(std::ostream &os,
                                 const char   *s,
                                 const size_t  n,
                                 const size_t  width )
    {
        assert(!(s==NULL&&n>0));
        for(size_t i=0;i<n;++i)
        {
            os << s[i];
        }
        for(size_t i=n;i<width;++i)
        {
            os << ' ';
        }
        return os;
    }

    std::ostream & display_align( std::ostream &os, const char   *s, const size_t width )
    {
        return display_align(os, s, length_of(s), width );
    }

    std::ostream & display_align( std::ostream &os, const string &s, const size_t width )
    {
        return display_align(os, *s, s.size(), width);
    }

}


