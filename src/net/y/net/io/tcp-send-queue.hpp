
//! \file
#ifndef Y_NET_IO_TCP_SEND_QUEUE_INCLUDED
#define Y_NET_IO_TCP_SEND_QUEUE_INCLUDED 1

#include "y/net/io/tcp-queue.hpp"

namespace upsylon
{
    namespace net
    {

        //! tcp queue to recieve data
        class tcp_send_queue : public tcp_queue
        {
        public:
            virtual ~tcp_send_queue() throw();        //!< cleanup
            explicit tcp_send_queue(const size_t bs); //!< setup



        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_send_queue);
        };
    }
}

#endif

