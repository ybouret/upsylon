#include "y/net/io/tcp-send-queue.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_send_queue:: ~tcp_send_queue() throw()
        {
        }

        tcp_send_queue:: tcp_send_queue(const size_t bs) : tcp_queue(bs)
        {
        }

    }

}

