
#include "y/net/comm/queues.hpp"
#include "y/net/comm/cache.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queues:: ~comm_queues() throw()
        {
        }


        static inline comm_block * __comm_block(const size_t block_exp2)
        {
            static comm_cache &mgr = comm_cache::instance();
            return mgr.query(block_exp2);
        }

        comm_queues:: comm_queues(const size_t send_block_exp2,
                                  const size_t recv_block_exp2) :
        sendQ( __comm_block(send_block_exp2) ),
        recvQ( __comm_block(recv_block_exp2) )
        {

        }

    }

}
