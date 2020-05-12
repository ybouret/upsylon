
#include "y/mpi++/mpi.hpp"
#include "y/os/rt-clock.hpp"
#include "y/exceptions.hpp"
#include "y/memory/buffers.hpp"
#include <cstdarg>
#include <cerrno>

namespace upsylon {
    
    
    void mpi:: Printf0(FILE *fp,const char *fmt,...) const
    {
        Barrier();
        if(head)
        {
            assert(fp);
            assert(fmt);
            //fprintf(fp,"%s> ", *processorName);
            va_list args;
            va_start(args, fmt);
            vfprintf(fp,fmt, args);
            va_end(args);
            fflush(fp);
        }
    }
    
    void mpi:: Printf(FILE *fp, const char *fmt, ...) const
    {
        typedef memory::buffer_of<char,memory::global> dataBuffer;
        
        Barrier();
        
        if(head)
        {
            assert(fp);
            assert(fmt);
            fprintf(fp,"%s> ", *nodeName);
            va_list args;
            va_start(args, fmt);
            vfprintf(fp,fmt, args);
            va_end(args);
            for(int r=1;r<size;++r)
            {
                const string peerName = Recv<string>(r);
                const string content  = Recv<string>(r);
                fprintf(fp,"%s> ", *peerName);
                fprintf(fp,"%s",   *content );

            }
            fflush(fp);
        }
        else
        {
            size_t n = 32;
        TRY:
            {
                dataBuffer databuf( n );
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
                    const string content = string( buffer, result );
                    Send(nodeName,0);
                    Send(content,0);
                    return;
                }
                n <<= 1;
            }
            goto TRY;

        }
    }
}


