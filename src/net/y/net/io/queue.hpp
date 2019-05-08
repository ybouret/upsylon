#ifndef Y_NET_IO_QUEUE_INCLUDED
#define Y_NET_IO_QUEUE_INCLUDED 1

#include "y/net/net.hpp"

namespace upsylon
{
    namespace net
    {

        class ioQ : public net_object
        {
        public:
            explicit ioQ();
            virtual ~ioQ() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ioQ);
        };

    }
}

#endif

