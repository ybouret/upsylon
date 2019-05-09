#ifndef Y_NET_IO_QUEUE_INCLUDED
#define Y_NET_IO_QUEUE_INCLUDED 1

#include "y/net/net.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace net
    {

        class io_queue : public net_object
        {
        public:
            explicit io_queue();
            virtual ~io_queue() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_queue);
        };

        typedef arc_ptr<io_queue> ioQ;

    }
}

#endif

