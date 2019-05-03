
#include "y/net/io/tcp-client.hpp"


namespace upsylon
{
    namespace net
    {
        tcp_client_protocol:: ~tcp_client_protocol() throw() {}

        tcp_client_protocol:: tcp_client_protocol( const tcp_link &lnk ) throw() :
        tcp_link(lnk)
        {
        }

        const size_t & tcp_client_protocol:: key() const throw()
        {
            const tcp_client &self = **this;
            return self.hkey;
        }

    }
}
