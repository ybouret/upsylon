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
        block_size(check_block_size(bs)),
        allocated(block_size),
        buffer( memory::pooled::instance().acquire_bytes(allocated) ),
        bpool()
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
            static const network &nw = network::instance();
            return byte_node_fmt( (bpool.size>0) ? bpool.query() : nw.acquire_byte_node() ,code);
        }



    }
}

namespace upsylon
{
    namespace net
    {

        tcp_recv_queue:: tcp_recv_queue( const size_t bs ) :
        tcp_queue(bs)
        {
        }

        tcp_recv_queue:: ~tcp_recv_queue() throw()
        {
        }

        size_t tcp_recv_queue:: size() const throw()
        {
            return bytes.size;
        }

        size_t tcp_recv_queue:: load(const tcp_client &client)
        {
            const size_t nr = client.recv(buffer,block_size);
            uint8_t     *p  = buffer;
            for(size_t i=nr;i>0;--i)
            {
                bytes.push_back( to_node( *p ) );
                *(p++) = 0;
            }
            return nr;
        }

        size_t tcp_recv_queue::  pop(void *ptr,size_t len) throw()
        {
            assert( !(NULL==ptr&&len>0) );
            size_t   ng   = 0;
            uint8_t *dest = static_cast<uint8_t *>(ptr);
            while(len-->0)
            {
                if( bytes.size <= 0 )
                {
                    break;
                }
                else
                {
                    byte_node *node = bpool.store( bytes.pop_front() );
                    dest[ng++]      = node->code;
                    node->code      = 0;
                }
            }
            return ng;
        }

        size_t tcp_recv_queue:: peek(void *ptr,size_t len) const throw()
        {
            assert( !(NULL==ptr&&len>0) );
            size_t           ng   = 0;
            uint8_t         *dest = static_cast<uint8_t *>(ptr);
            const byte_node *node = bytes.head;
            while(len-->0)
            {
                if(!node) break; else dest[ng++] = node->code;
                node = node->next;
            }
            return ng;
        }

        char tcp_recv_queue:: getch() throw()
        {
            assert(bytes.size>0);
            char C = char(bytes.head->code);
            bpool.store( bytes.pop_front() )->code = 0;
            return C;
        }

        void tcp_recv_queue:: putch(char C)
        {
            bytes.push_front( to_node(C) );
        }

    }

}

