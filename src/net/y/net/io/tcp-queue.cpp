#include "y/net/io/tcp-queue.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/type/block/zset.hpp"

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
        block_size(check_block_size(bs)),
        allocated(block_size),
        buffer( memory::pooled::instance().acquire_bytes(allocated) ),
        bpool()
        {
        }

        tcp_queue:: ~tcp_queue() throw()
        {
            prune();


            while( bytes.size )
            {
                static network &nw = network::location();
                nw.byte_nodes.release( bytes.pop_back() );
            }


            memset(buffer,0,allocated);
            memory::pooled::location().release_bytes(buffer,allocated);
            _bzset(block_size);
        }

        void tcp_queue:: clear() throw()
        {
            while(bytes.size) bpool.store( bytes.pop_back() )->code = 0;
            memset(buffer,0,block_size);
        }

        void tcp_queue:: prune() throw()
        {
            while( bpool.size )
            {
                static   network &nw = network::location();
                nw.byte_nodes.release( bpool.query() );
            }
        }

        namespace
        {
            static inline byte_node *byte_node_fmt( byte_node *node, const uint8_t code ) throw()
            {
                assert(node);
                node->code = code;
                return node;
            }
        }


        byte_node * tcp_queue:: to_node(const uint8_t code)
        {
            static network &nw = network::instance();
            return byte_node_fmt( (bpool.size>0) ? bpool.query() : nw.byte_nodes.acquire() ,code);
        }



    }
}

