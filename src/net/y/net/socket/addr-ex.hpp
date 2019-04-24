
// !\file
#ifndef Y_NET_SOCKET_ADDR_EX_INCLUDED
#define Y_NET_SOCKET_ADDR_EX_INCLUDED 1

#include "y/net/socket/address.hpp"

namespace upsylon
{
    namespace net
    {

        //! smart pointer for address
        class socket_addr_ex
        {
        public:
            virtual ~socket_addr_ex() throw();
            explicit socket_addr_ex( const socket_address &ip ) throw();
            explicit socket_addr_ex( const string         &xname, const ip_version version);
            explicit socket_addr_ex( const char           *xname, const ip_version version);

            socket_address       & operator*() throw();
            const socket_address & operator*() const throw();

            socket_address       * operator->() throw();
            const socket_address * operator->() const throw();

        private:
            uint64_t        wksp[ Y_U64_FOR_ITEM(ipv6) ];
            socket_address *addr;

            void setup( const socket_address &ip ) throw();
            void setup( const string &xname, const ip_version version);

        };

    }

}

#endif

