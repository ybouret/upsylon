
//! \file
#ifndef Y_NET_IO_RECV_QUEUE_INCLUDED
#define Y_NET_IO_RECV_QUEUE_INCLUDED 1

#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! transform block input into queue of byte
        //
        //______________________________________________________________________
        class recv_queue : public io_queue
        {
        public:
            explicit recv_queue(const size_t bs); //!< setup data/queue
            virtual ~recv_queue() throw();        //!< cleanup

        protected:
            void load(size_t bs); //!< data --> queue

        private:
            Y_DISABLE_COPY_AND_ASSIGN(recv_queue);
        };

    }

}

#endif
