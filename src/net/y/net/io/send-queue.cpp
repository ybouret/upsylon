
#include "y/net/io/send-queue.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace net
    {

        send_queue:: ~send_queue() throw()
        {

        }

        send_queue:: send_queue(const size_t bs) : io_queue(bs)
        {

        }
        
    }

}

