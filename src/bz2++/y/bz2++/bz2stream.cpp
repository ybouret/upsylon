#include "y/bz2++/bz2stream.hpp"
#include "y/exception.hpp"

#include <iostream>

namespace upsylon
{
    namespace bzlib
    {
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        
        bz2stream:: ~bz2stream() throw()
        {
            bzfile = 0;
        }
        
        
        bz2stream:: bz2stream() throw() :
        bzerror( BZ_OK ),
        bzfile(0)
        {
        }
        
        const char *bz2stream:: get_error() const throw()
        {
            int errnum = BZ_OK;
            return BZ2_bzerror( (BZFILE *)bzfile, &errnum);
        }

        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        ibz2stream:: ibz2stream( const string &filename ) :
        bz2stream(),
        ios::istream(),
        fp( filename ),
        cache(-1),
        alive(true),
        last_close(0)
        {
            bzfile = BZ2_bzReadOpen( &bzerror, *fp, 0, 0, 0, 0);
            if( !bzfile )
                throw exception("BZ2_bzReadOpen(%s)/%s", get_error(), *filename);
        }
        
        ibz2stream:: ibz2stream( const ios::cstdin_t &_ ) :
        bz2stream(),
        ios::istream(),
        fp( _ ),
        cache(-1),
        alive(true),
        last_close(0)
        {
            bzfile = BZ2_bzReadOpen( &bzerror, *fp, 0, 0, 0, 0);
            if( !bzfile )
                throw exception("BZ2_bzReadOpen(%s)/stdin",get_error());
        }
        
        ibz2stream:: ~ibz2stream() throw()
        {
            int * err = &bzerror;
            if( last_close )
            {
                err  = last_close;
                *err = BZ_OK;
            }
            BZ2_bzReadClose(err,bzfile);
        }
        
        void ibz2stream:: store( char  C )
        {
            if( cache < 0 )
                cache = uint8_t(C);
            else
                throw exception("ibz2stream.store(IMPOSSIBLE)");
        }
        
        bool ibz2stream:: query( char &C )
        {
            if( cache >= 0 )
            {
                C = char( uint8_t(cache) );
                cache = -1;
                return true;
            }
            else
            {
                if(alive)
                {
                    const int res = BZ2_bzRead(&bzerror, bzfile, &C, 1);
                    switch( bzerror )
                    {
                        case BZ_STREAM_END:
                            alive=false; /* FALLTHRU */
                        case BZ_OK:
                            return res>0;
                        default:
                            break;
                    }
                    throw exception("BZ2_bzRead(%s)",get_error());
                }
                else
                    return false;
                
            }
        }
        
        
        
    }
}
