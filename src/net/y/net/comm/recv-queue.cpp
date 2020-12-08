#include "y/net/comm/recv-queue.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace net
    {

        recv_queue:: ~recv_queue() throw()
        {

        }

        recv_queue:: recv_queue(const size_t bs) : io_queue(bs)
        {

        }

        size_t recv_queue:: fetch_(size_t bs)
        {
            if(bs>0)
            {
                assert(bs<=data.length());
                if(bs>pool.size) reserve(bs-pool.size); assert(pool.size>=bs);

                uint8_t *bp = data.as<uint8_t>();
                while(bs-- > 0 )
                {
                    assert(pool.size>0);
                    aliasing::_( push_back( pool.pop_back() )->code ) = *(bp++);
                }
                return bs;
            }
            else
            {
                return 0;
            }

        }

        void recv_queue:: reset_() throw()
        {
        }
        
    }

}

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {
        size_t recv_queue:: dowload(tcp_client &client)
        {
            return fetch_( client.recv( data.rw(), data.length() ) );
        }


    }
}


