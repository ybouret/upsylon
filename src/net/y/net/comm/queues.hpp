
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

        //______________________________________________________________________
        //
        //
        //! send/recv queues
        //
        //______________________________________________________________________
        class comm_queues
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup with send/recv block size
            explicit comm_queues(const size_t send_block_exp2,
                                 const size_t recv_block_exp2);

            //! cleanup
            virtual ~comm_queues() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            send_queue sendQ; //!< to send
            recv_queue recvQ; //!< to recv
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_queues);
        };

        
    }

}

#endif
