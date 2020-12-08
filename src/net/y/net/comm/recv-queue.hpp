
//! \file
#ifndef Y_NET_COMM_RECV_QUEUE_INCLUDED
#define Y_NET_COMM_RECV_QUEUE_INCLUDED 1

#include "y/net/comm/queue.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //! forward declaration
        //______________________________________________________________________
        class tcp_client;

        //______________________________________________________________________
        //
        //
        //! transform block input into queue of byte
        //
        //______________________________________________________________________
        class recv_queue : public comm_queue
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit recv_queue(const size_t bs); //!< setup data/queue
            virtual ~recv_queue() throw();        //!< cleanup

            size_t download(tcp_client &);               //!< receive from tcp_client
            size_t justLoad(const void *, const size_t); //!< copy and bring, to debug
            void   demoLoad(const void *, const size_t); //!< copy all and bring all
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(recv_queue);
            virtual void reset_() throw();  
            size_t       bring_(size_t bs); //!< data --> queue
        };

    }

}

#endif
