
#include "y/ios/icstream.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace ios
    {
        icstream:: ~icstream() throw()
        {
        }

        icstream:: icstream(const string &filename ) :
        handle( filename, ios::cfile::open_read )
        {
        }

        icstream:: icstream(const char *filename ) :
        handle( filename, ios::cfile::open_read )
        {
        }

        icstream:: icstream(const cstdin_t &_) :
        handle(_)
        {
        }

        
        FILE * icstream:: operator*() throw()
        {
            return *handle;
        }
        
        bool icstream:: query(char &C)
        {
            if( feof(*handle) )
            {
                return false;
            }
            else
            {
                if( 1 != fread( &C, 1, 1, *handle) )
                {
                    if( feof(*handle) )
                    {
                        return false;
                    }
                    throw libc::exception( EIO, "icstream::query()" );
                }
                else
                {
                    return true;
                }
            }

        }

        void icstream:: store( char C )
        {
            const int ans = C;
            if( ans != ungetc( C, *handle) )
            {
                throw libc::exception( EIO, "icstream::store" );
            }
        }

    }
}

