
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
        

        typedef arc_ptr<recv_queue>  recvQ;
        typedef arc_ptr<send_queue>  sendQ;

        class queues
        {
        public:
            explicit queues(const recvQ &r, const recvQ &s) throw();
            virtual ~queues() throw();

            recvQ recv;
            sendQ send;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(queues);
        };

    }

}

#endif
