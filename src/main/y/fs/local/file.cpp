#include "y/fs/local/file.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace upsylon
{
    namespace ios
    {
        static const char fn[] = "ios::local_file";

        descriptor::type  local_file:: operator*() throw()
        {
            return handle;
        }

        local_file:: ~local_file() throw()
        {
            switch(type)
            {
                case is_regular:
                case is_pipe: {
                    const int ans = descriptor::close(handle);
                    if(status) *status = ans;
                } break;
                default:
                    if(status) *status=0;
                    break;
            }
            handle = descriptor::invalid();
            (unsigned &)access = 0;
        }

        local_file::local_file( const string &filename,
                               const unsigned mode) :
        handle( descriptor::invalid()      ),
        access( mode & (readable|writable) ),
        type( is_regular ),
        status(NULL)
        {
#include "file.cxx"
        }

        local_file:: local_file( const char   *_,
                                const unsigned mode ) :

        handle( descriptor::invalid() ),
        access( mode & (readable|writable) ),
        type( is_regular ),
        status(NULL)
        {
            const string filename(_);
#include "file.cxx"
        }

        local_file:: local_file( handle_t user_handle, size_t user_access ) throw() :
        handle( user_handle ),
        access( user_access & (readable|writable) ),
        type(is_pipe),
        status(NULL)
        {
            assert( handle != descriptor::invalid() );
            assert( (access&readable) || (access&writable) );
        }

        local_file:: local_file( const ios::cstdin_t  & ) :
        handle( descriptor::invalid()  ),
        access( readable ),
        type( is_stdin ),
        status(NULL)
        {
#if defined(Y_BSD)
            handle = STDIN_FILENO;
#endif

#if defined(Y_WIN)
            handle = ::GetStdHandle(STD_INPUT_HANDLE);
#endif
        }

        local_file:: local_file( const ios::cstdout_t  & ) :
        handle( descriptor::invalid()  ),
        access( writable ),
        type( is_stdout ),
        status(NULL)
        {
#if defined(Y_BSD)
            handle = STDOUT_FILENO;
#endif

#if defined(Y_WIN)
            handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
        }

        local_file:: local_file( const ios::cstderr_t  & ) :
        handle( descriptor::invalid()  ),
        access( writable ),
        type( is_stdout ),
        status(NULL)
        {
#if defined(Y_BSD)
            handle = STDERR_FILENO;
#endif

#if defined(Y_WIN)
            handle = ::GetStdHandle(STD_ERROR_HANDLE);
#endif
        }

    }

}

namespace upsylon
{
    namespace ios
    {

        offset_t local_file:: seek( offset_t delta, whence_t whence )
        {
            Y_GIANT_LOCK();
#if defined(Y_WIN)
            LARGE_INTEGER mov;
            LARGE_INTEGER res;
            mov.QuadPart = delta;
            res.QuadPart = 0;
#endif

            switch( whence )
            {
                case from_set:
                {
#if defined(Y_BSD)
                    const offset_t res = lseek( handle, delta, SEEK_SET);
                    if( res < 0 )
                        throw libc::exception( errno, "lseek(SEEK_SET)");
                    return res;
#endif

#if defined(Y_WIN)
                    if( ! ::SetFilePointerEx( handle, mov, &res, FILE_BEGIN) )
                        throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_BEGIN)" );
                    return res.QuadPart;
#endif

                }

                case from_cur:
                {
#if defined(Y_BSD)
                    const offset_t res = lseek( handle, delta, SEEK_CUR);
                    if( res < 0 )
                        throw libc::exception( errno, "lseek(SEEK_CUR)");
                    return res;
#endif

#if defined(Y_WIN)
                    if( ! ::SetFilePointerEx( handle, mov, &res, FILE_CURRENT) )
                        throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_CURRENT)" );
                    return res.QuadPart;
#endif

                }

                case from_end:
                {
#if defined(Y_BSD)
                    const offset_t res = lseek( handle, delta, SEEK_END);
                    if( res < 0 )
                        throw libc::exception( errno, "lseek(SEEK_END)");
                    return res;
#endif

#if defined(Y_WIN)
                    if( ! ::SetFilePointerEx( handle, mov, &res, FILE_END) )
                        throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_END)" );
                    return res.QuadPart;
#endif
                }
            }
            return 0;
        }


        offset_t local_file:: tell()
        {
            return this->seek(0,from_cur);
        }

        void local_file:: rewind()
        {
            (void) this->seek(0,from_set);
        }

        void local_file:: unwind()
        {
            (void) this->seek(0,from_end);
        }

        size_t local_file:: length()
        {
            const offset_t now = this->tell();
            const offset_t ans = this->seek(0,from_end);
            if( now != this->seek( now, from_set ) )
                throw exception( "local_file::length(bad reset)");
            return size_t(ans);
        }
        
        size_t local_file:: length_of( const string &filename )
        {
            local_file fp( filename, readable);
            return fp.length();
        }
        
        size_t local_file:: length_of( const char *filename )
        {
            const string _(filename);
            return length_of(_);
        }


    }

}

