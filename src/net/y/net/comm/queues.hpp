
//! \file
#ifndef Y_NET_COMM_QUEUES_INCLUDED
#define Y_NET_COMM_QUEUES_INCLUDED 1

#include "y/net/comm/send-queue.hpp"
#include "y/net/comm/recv-queue.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace net
    {
        
        class comm_queues
        {
        public:
            explicit comm_queues(const size_t send_block_size,
                                 const size_t recv_block_size);
            virtual ~comm_queues() throw();

            send_queue sendQ;
            recv_queue recvQ;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_queues);
        };

        
    }

}

#endif
