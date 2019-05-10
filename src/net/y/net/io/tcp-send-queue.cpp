#include "y/net/io/tcp-send-queue.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_send_queue:: ~tcp_send_queue() throw()
        {
        }

        tcp_send_queue:: tcp_send_queue(const size_t bs) :
        tcp_queue(bs),
        current(buffer),
        to_send(0),
        available(buffer),
        remaining(block_size)
        {
        }

        void tcp_send_queue:: reset() throw()
        {
            clear();
            current   = buffer;
            to_send   = 0;
            available = buffer;
            remaining = block_size;
        }

        size_t     tcp_send_queue::    buffer_space() const throw()
        {
            return block_size - to_send;
        }

        void tcp_send_queue:: defrag() throw()
        {
            assert(to_send<=block_size);
            {
                uint8_t     *target  = buffer;
                uint8_t     *source  = current;
                for(size_t i=to_send;i>0;--i);
                {
                    *(target++) = *(source++);
                }
            }
            available = buffer+to_send;
            remaining = buffer_space();

            while(remaining>0&&bytes.size>0)
            {
                *(available++) = bpool.store(bytes.pop_front())->code;
                --remaining;
                ++to_send;
            }
        }

        size_t tcp_send_queue:: size() const throw()
        {
            return to_send + bytes.size;
        }

        void tcp_send_queue:: push(const void *ptr, size_t num)
        {
            const uint8_t *p = static_cast<const uint8_t *>(ptr);

            // defrag ?
            if( (num>>1) >= buffer_space() )
            {
                defrag();
            }

            // fill in remaining
            while(num>0&&remaining>0)
            {
                *(available++) = *(p++);
                --num;
                --remaining;
                ++to_send;
            }

            // enqueue remaining bytes
            while(num-->0)
            {
                bytes.push_back( to_node(*(p++)) );
            }

        }

        void tcp_send_queue:: push( const char C )
        {
            push(&C,1);
        }

        void tcp_send_queue:: push(const char *text)
        {
            push( text, length_of(text) );
        }

        void tcp_send_queue:: push( const memory::ro_buffer &buff )
        {
            push( buff.ro(), buff.length() );
        }

        size_t tcp_send_queue:: comm(const tcp_client &cln)
        {
            const size_t ns = cln.send(buffer,to_send);

            current += ns;
            to_send -= ns;
            defrag();

            return ns;
        }
        
        bool tcp_send_queue:: is_filled() const throw()
        {
            return remaining<=0;
        }
    }

}

#include "y/code/utils.hpp"

namespace upsylon
{
    namespace net
    {

        std::ostream & tcp_send_queue:: display( std::ostream &os ) const
        {
            os << '[';
            for(size_t i=0;i<to_send;++i)
            {
                os << visible_char[ buffer[i] ];
            }
            os <<']' << '{';
            for(const byte_node *node = bytes.head; node; node=node->next)
            {
                os << visible_char[ node->code ];
            }
            os << '}';
            return os;
        }

    }

}

