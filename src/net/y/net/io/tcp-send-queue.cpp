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

        void tcp_send_queue:: defrag() throw()
        {
            assert(to_send<=block_size);
            const size_t to_move = to_send;
            for(size_t i=0;i<to_move;++i)
            {
                buffer[i] = current[i];
            }
            available = buffer+to_send;
            remaining = block_size-to_send;
        }

        size_t tcp_send_queue:: size() const throw()
        {
            return to_send + bytes.size;
        }

        void tcp_send_queue:: push(const void *ptr, size_t num)
        {
            const uint8_t *p = static_cast<const uint8_t *>(ptr);

            // defrag ?


            // fill in remaining
            while(num>0&&remaining>0)
            {
                *(available++) = *(p++);
                --num;
                --remaining;
            }

            // enqueue remaining bytes
            while(num-->0)
            {
                bytes.push_back( to_node(*(p++)) );
            }

        }

        void tcp_send_queue:: putch( const char C )
        {
            push(&C,1);
        }
    }

}

