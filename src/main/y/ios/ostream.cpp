#include "y/ios/ostream.hpp"
#include "y/memory/buffers.hpp"
#include "y/exceptions.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"
#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace upsylon
{
    namespace ios
    {
        ostream:: ~ostream() throw() {}
        ostream::  ostream() throw() {}

        ostream & ostream:: operator<<(const char C)
        {
            write(C);
            return *this;
        }

        ostream & ostream:: operator<<(const memory::ro_buffer &buffer)
        {
            output( static_cast<const char *>(buffer.ro()), buffer.length() );
            return *this;
        }

        void ostream:: output(const char *buffer, const size_t buflen)
        {
            assert(!(buffer==0&&buflen>0));
            for(size_t i=0;i<buflen;++i) write(buffer[i]);
        }

        ostream & ostream:: operator()(const char *fmt,...)
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
                    return *this;
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

        size_t ostream:: emit_block( const void *data, const size_t size )
        {
            assert( !(NULL==data&&size>0) );
            size_t shift = 0;
            emit_upack(size,&shift).output( (const char *)data,size);
            return shift + size;
        }

        size_t ostream:: emit_block( const memory::ro_buffer  &buff )
        {
            return emit_block(buff.ro(), buff.length());
        }

        size_t ostream:: emit_block( const char *text )
        {
            return emit_block( text, length_of(text) );
        }

        ostream & ostream:: operator<<( const char *buffer )
        {
            output(buffer,length_of(buffer));
            return *this;
        }

        ostream & ostream:: align(const char *text,const size_t aligned,const char C)
        {
            return align(text,length_of(text),aligned,C);
        }

        ostream & ostream:: align(const string &s,const size_t aligned,const char C)
        {
            return align(*s,s.size(),aligned,C);
        }

    }

}

