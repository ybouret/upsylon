//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {
        class engine : public net_object
        {
        public:
            explicit engine();
            virtual ~engine() throw();

        private:
            socket_set sockset;
            Y_DISABLE_COPY_AND_ASSIGN(engine);
        };
    }
}

#endif

