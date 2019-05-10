#include "y/net/io/tcp-recv-queue.hpp"

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

        size_t tcp_recv_queue:: comm(const tcp_client &client)
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

        void tcp_recv_queue:: reset() throw()
        {
            clear();
        }

        void    tcp_recv_queue:: skip(size_t num) throw()
        {
            assert(num<=bytes.size);
            while(num-->0)
            {
                bpool.store( bytes.pop_front() )->code = 0;
            }
        }
    }

}



