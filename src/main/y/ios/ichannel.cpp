
#include "y/ios/ichannel.hpp"
#include "y/type/utils.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace ios
    {
        bulk:: ~bulk() throw()
        {
            memory::global::location().release( *(void **)&addr, size);
        }

        bulk:: bulk(const size_t n) :
        size( memory::align(max_of<size_t>(n,1)) ),
        addr( static_cast<char *>(memory::global::instance().acquire(size) ))
        {
        }
        
    }

}

#include <cstdio>
#include <cerrno>

namespace upsylon
{
    namespace ios
    {

#define Y_ICHANNEL_CTOR() curr(0), last(0), io(buff)
        ichannel:: ~ichannel() throw()
        {
        }

        ichannel:: ichannel(const string &filename,
                            const bulk::pointer &buff,
                            const offset_t shift ) :
        Y_ICHANNEL_CTOR(),
        fp(filename,readable)
        {
            (void) fp.seek(shift,from_set);
        }

        ichannel:: ichannel(const char *filename,
                            const bulk::pointer &buff,
                            const offset_t shift ) :
        Y_ICHANNEL_CTOR(),
        fp(filename,readable)
        {
            (void) fp.seek(shift,from_set);
        }

        ichannel:: ichannel( const cstdin_t     &_ ,
                            const bulk::pointer &buff) :
        Y_ICHANNEL_CTOR(),
        fp(_)
        {
        }


        bool ichannel:: query(char &C)
        {
            if(curr>=last)
            {
                if(!reload())
                {
                    return false;
                }
                assert(curr==io->addr);
                assert(curr<last);
            }

            C = *(curr++);
            return true;
        }

        void ichannel:: store(char C)
        {
            if( NULL == curr || (curr<=io->addr) )
            {
                throw libc::exception( EIO, "unable to ichannel::store");
            }
            *(--curr) = C;
        }

        bool ichannel:: reload()
        {
            assert(curr>=last);
            curr = last = 0;
            size_t done = 0;
            descriptor::get(*fp,io->addr, io->size, done);
            if(done)
            {
                curr = io->addr;
                last = curr+done;
                return true;
            }
            else
            {
                return false;
            }
        }

    }

}


