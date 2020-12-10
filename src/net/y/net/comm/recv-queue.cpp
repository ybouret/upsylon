#include "y/net/comm/recv-queue.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include <cstring>

namespace upsylon
{
    namespace net
    {

        recv_queue:: ~recv_queue() throw()
        {

        }

        recv_queue:: recv_queue(const size_t bs) : comm_queue(bs)
        {

        }

        size_t recv_queue:: bring_(size_t bs)
        {
            const size_t res = bs;
            if(bs>0)
            {
                assert(bs<=data->size);
                if(bs>pool.size) reserve(bs-pool.size); assert(pool.size>=bs);

                uint8_t *bp = **data;
                while(bs-- > 0 )
                {
                    assert(pool.size>0);
                    aliasing::_( push_back( pool.pop_back() )->code ) = *bp;
                    *(bp++) = 0;
                }
                return res;
            }
            else
            {
                return 0;
            }

        }

        void recv_queue:: reset_() throw()
        {
        }


        size_t recv_queue:: justLoad(const void *buffer, const size_t buflen)
        {
            assert(!(NULL==buffer&&buflen>0));
            const size_t bs = min_of(buflen,data->size);
            memcpy( **data, buffer, bs);
            return bring_(bs);
        }

        void   recv_queue:: demoLoad(const void *buffer, const size_t buflen)
        {
            assert(!(NULL==buffer&&buflen>0));
            const uint8_t *base = static_cast<const uint8_t*>(buffer);
            size_t         todo = buflen;
            while(todo)
            {
                const size_t ld = justLoad(base,todo);
                assert(ld<=todo);
                todo -= ld;
                base += ld;
            }
        }

        void recv_queue:: demoLoad(const char *text)
        {
            if(text) demoLoad( text, strlen(text) );
        }


        bool recv_queue:: is_active() throw()
        {
            return size>0;
        }

        bool recv_queue:: query(char &C)
        {
            if(size>0)
            {
                C = char( pop() );
                return true;
            }
            else
            {
                return false;
            }
        }

        void recv_queue:: store(char C)
        {
            push_front( rig(C) );
        }


    }

}

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {
        size_t recv_queue:: download(tcp_client &client)
        {
            return bring_( client.recv( **data, data->size ) );
        }
    }
}


