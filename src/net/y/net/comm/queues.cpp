
#include "y/net/comm/queues.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queues:: ~comm_queues() throw()
        {
        }

        comm_queues:: comm_queues(const size_t send_block_size,
                                  const size_t recv_block_size) :
        sendQ(send_block_size),
        recvQ(recv_block_size)
        {
        }


    }

}
