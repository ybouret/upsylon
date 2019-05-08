
//! \file
#ifndef Y_NET_IO_TCP_CLIENT_INCLUDED
#define Y_NET_IO_TCP_CLIENT_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/net/socket/id-hasher.hpp"
#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {

        class tcp_client_protocol : public tcp_link
        {
        public:
            virtual ~tcp_client_protocol() throw();
            explicit tcp_client_protocol(const tcp_link &link) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_client_protocol);
        };

    }

}

#endif

