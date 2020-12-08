#include "y/net/io/recv-queue.hpp"
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

        void recv_queue:: load(size_t bs)
        {
            assert(bs<=data.length());
            if(bs>pool.size) reserve(bs-pool.size); assert(pool.size>=bs);

            uint8_t *bp = data.as<uint8_t>();
            while(bs-- > 0 )
            {
                assert(pool.size>0);
                aliasing::_( push_back( pool.pop_back() )->code ) = *(bp++);
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
        void recv_queue:: recv(tcp_client &client)
        {
            load( client.recv( data.rw(), data.length() ) );
        }


    }
}

#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        void recv_queue:: recv(udp_client &client)
        {
            load( client.recv( data.rw(), data.length() ) );
        }
    }
}
