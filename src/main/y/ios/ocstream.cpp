
#include "y/ios/ocstream.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <cstdarg>

namespace upsylon
{
    namespace ios
    {
        ocstream:: ~ocstream() throw()
        {

        }

        ocstream:: ocstream(const string &filename, const bool append ) :
        handle( filename, append ? ios::cfile::open_append : ios::cfile::open_write )
        {
        }

        ocstream:: ocstream(const char *filename, const bool append ) :
        handle( filename, append ? ios::cfile::open_append : ios::cfile::open_write )
        {
        }

        ocstream:: ocstream( const ios::cstderr_t &_) :
        handle(_)
        {
        }

        ocstream:: ocstream( const ios::cstdout_t &_) :
        handle(_)
        {
        }

        void ocstream:: write( char C )
        {
            if( 1 != fwrite( &C, 1, 1, *handle ) )
                throw libc::exception( errno, "ocstream::write(%d)", int(C) );

        }

        void ocstream:: flush()
        {
            if( fflush(*handle) != 0 )
            {
                throw libc::exception( EIO, "ocstream::flush()");
            }
        }


        ostream & ocstream:: operator()(const char *format,...)
        {
            assert(format);
            va_list args;
            va_start(args, format);
            vfprintf(*handle, format, args);
            va_end (args);
            if( ferror(*handle) != 0 )
                throw libc::exception( EIO, "ocstream('%s',...)", format );
            return *this;
        }

        void ocstream:: overwrite( const string &filename )
        {
            ios::ocstream fp(filename,false);
            fp.flush();
        }

        void ocstream:: overwrite( const char *filename)
        {
            const string _(filename); overwrite(_);
        }

        void ocstream:: echo(const string &filename, const char *format,...)
        {
            ios::ocstream fp(filename,true);
            assert(format);
            va_list args;
            va_start(args, format);
            vfprintf(*fp.handle, format, args);
            va_end (args);
            if( ferror(*fp.handle) != 0 )
                throw libc::exception( EIO, "ocstream::echo('%s',...)", *filename );
        }

        void ocstream:: echo(const char *filename, const char *format,...)
        {
            const string _(filename);
            ios::ocstream fp(_,true);
            assert(format);
            va_list args;
            va_start(args, format);
            vfprintf(*fp.handle, format, args);
            va_end (args);
            if( ferror(*fp.handle) != 0 )
                throw libc::exception( EIO, "ocstream::echo('%s',...)", *_ );
        }



        FILE * ocstream:: operator*() throw()
        {
            return *handle;
        }

    }

}

