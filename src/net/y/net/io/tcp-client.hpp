//! \file
#ifndef Y_NET_IO_TCP_CLIENT_INCLUDED
#define Y_NET_IO_TCP_CLIENT_INCLUDED 1


#include "y/net/tcp/client.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace net
    {
        //! handle tcp client connection
        class tcp_client_protocol : public net_object, public tcp_link
        {
        public:
            explicit tcp_client_protocol(const tcp_link &) throw(); //!< setup
            virtual ~tcp_client_protocol() throw();                 //!< cleanup

            const size_t &key() const throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_client_protocol);
        };

        //! dynamic tcp_server_protocol
        typedef intr_ptr<size_t,tcp_client_protocol> tcp_client_proto;

    }
}

#endif

