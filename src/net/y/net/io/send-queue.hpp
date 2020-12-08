//! \file
#ifndef Y_NET_IO_SEND_QUEUE_INCLUDED
#define Y_NET_IO_SEND_QUEUE_INCLUDED 1

#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! pack user bytes into block
        //
        //______________________________________________________________________
        class send_queue : public io_queue
        {
        public:
            explicit send_queue(const size_t bs); //!< setup data/queue
            virtual ~send_queue() throw();        //!< cleanup

        protected:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(send_queue);
        };

    }

}

#endif
