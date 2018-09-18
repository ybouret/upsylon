#include "y/ios/ostream.hpp"
#include "y/memory/buffers.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace ios
    {
        ostream:: ~ostream() throw() {}
        ostream::  ostream() throw() {}


        void ostream:: operator()(const char *fmt,...)
        {
            assert(fmt);
            size_t n = 32;
        TRY:
            {
                memory::buffer_of<char,memory::pooled> databuf( n );
                char  *buffer  = *databuf;
                int    length  = int( databuf.length() );
                if( length < 0 )
                    throw libc::exception( ERANGE, "ostream(...) memory overflow");

                va_list ap;
                va_start( ap, fmt );
                const int  result  = vsnprintf( buffer, length, fmt, ap);
                const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
                va_end (ap);

                if( success )
                {
                    output( buffer, length_of(buffer) );
                    return;
                }
                n <<= 1;
            }
            goto TRY;

        }

        ostream & ostream:: viz( const void *addr )
        {
            write('a');
            uint8_t block[ sizeof(void *) ];
            memcpy(block, &addr, sizeof(void*) );
            for(size_t i=0;i<sizeof(block);++i)
            {
                output(hexadecimal::uppercase[block[i]],2);
            }
            return *this;
        }
    }

}

