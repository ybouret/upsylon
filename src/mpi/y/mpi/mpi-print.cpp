#include "y/mpi/mpi.hpp"
#include "y/exceptions.hpp"
#include "y/memory/buffers.hpp"
#include <cstdarg>
#include <cstdio>

namespace upsylon
{
    void mpi:: print0( FILE *fp, const char *format,...)
    {
        static const char fn[] = "mpi::print0: ";
        if(isHead)
        {
            assert(fp);
            va_list args;
            va_start(args, format);
            vfprintf(fp, format, args);
            va_end (args);
            if( ferror(fp) != 0 )
            {
                throw libc::exception( EIO, "%sformat='%s'", fn, format );
            }
        }
    }


    bool mpi:: write_to(FILE *fp, const string &text) const throw()
    {
        const size_t n = text.size();
        return n == fwrite( &text[0], 1, n, fp) ;
    }



    void mpi:: print( FILE *fp, const char *format, ... )
    {
        static const char fn[]  = "mpi::print: ";

        if(isHead)
        {
            assert(fp);
            assert(format);

            // write prolog for head
            if( !write_to(fp,nodeName)) throw libc::exception( EIO, "%swriting rank#0 name",fn);

            va_list args;
            va_start(args, format);
            vfprintf(fp, format, args);
            va_end (args);
            if( ferror(fp) != 0 )
            {
                throw libc::exception( EIO, "%sformat='%s'", fn, format );
            }

            // then receive strings
            for(int r=1;r<size;++r)
            {
                {
                    const string tmp = Recv<string>(r,io_tag);
                    if(!write_to(fp,tmp)) throw libc::exception( EIO, "%swriting rank#%d name", fn, r);
                }

                {
                    const string tmp = Recv<string>(r,io_tag);
                    if(!write_to(fp,tmp)) throw libc::exception( EIO, "%swriting content from rank#%d", fn, r);
                }
            }
        }
        else
        {
            size_t n = 64;
        TRY:
            {
                memory::buffer_of<char,memory::pooled> databuf( n );
                char  *buffer  = *databuf;
                int    length  = int( databuf.length() );
                if( length < 0 )
                    throw libc::exception( ERANGE, "%s(...) memory overflow", fn);

                va_list ap;
                va_start( ap, format );
                const int  result  = vsnprintf( buffer, length, format, ap);
                const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
                va_end (ap);

                if( success )
                {
                    Send<string>(nodeName,0,io_tag);
                    const string tmp(buffer,length_of(buffer));
                    Send<string>(tmp,0,io_tag);
                    return;
                }
                n <<= 1;
            }
            goto TRY;
        }

    }

    void mpi:: flush( FILE *fp )
    {
        if(isHead)
        {
            assert(fp);
            fflush(fp);
        }
    }

}


