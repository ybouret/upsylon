#include "y/net/io/queue.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace net
    {


        static size_t check_block_size(size_t bs) throw()
        {
            if(bs<=0)
            {
                bs = 1;
            }

            return Y_ROUND8(bs);
        }

        tcp_queue:: tcp_queue( const size_t bs) :
        net_object(),
        bytes(),
        bpool(),
        block_size(check_block_size(bs)),
        allocated(block_size),
        buffer( memory::pooled::instance().acquire_bytes(allocated) )
        {
        }

        tcp_queue:: ~tcp_queue() throw()
        {
            trim();

            while( bytes.size )
            {
                static const network &nw = network::location();
                nw.release_byte_node( bytes.pop_back() );
            }



            memset(buffer,0,allocated);
            memory::pooled::location().release_bytes(buffer,allocated);
            (size_t&)block_size=0;
        }

        void tcp_queue:: free() throw()
        {
            while(bytes.size) bpool.store( bytes.pop_back() )->code = 0;
        }

        void tcp_queue:: trim() throw()
        {
            while( bpool.size )
            {
                static const network &nw = network::location();
                nw.release_byte_node( bpool.query() );
            }
        }


        byte_node * tcp_queue:: to_node(const uint8_t code)
        {
            if(bpool.size>0)
            {
                byte_node *node = bpool.query();
                node->code = code;
                return node;
            }
            else
            {
                static const network &nw = network::instance();
                byte_node *node = nw.acquire_byte_node();
                node->code = 0;
                return node;
            }
        }


        size_t tcp_queue:: recv( tcp_client &client )
        {
            const size_t nr = client.recv(buffer,block_size);
            for(size_t i=0;i<nr;++i)
            {
                bytes.push_back( to_node( buffer[i] ) );
            }
            return nr;
        }


    }
}
