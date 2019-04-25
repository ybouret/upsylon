//! \file
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
            virtual ~socket_addr_ex() throw();                                                         //!< destructor
            explicit socket_addr_ex(const socket_address &) throw();                                   //!< initialize to a precomputed address
            explicit socket_addr_ex(const ip_addr_value, const ip_version , const uint16_t=0) throw(); //!< initialize to a specific address
            explicit socket_addr_ex(const string &, const ip_version version);                         //!< initialize by name resolution
            explicit socket_addr_ex(const char   *, const ip_version version);                         //!< initialize by name resoklution

            socket_address       & operator*() throw();        //!< content operator
            const socket_address & operator*() const throw();  //!< content operator, const
            socket_address       * operator->() throw();       //!< transitive indirection
            const socket_address * operator->() const throw(); //!< transitive indirection, const

        private:
            uint64_t        wksp[ Y_U64_FOR_ITEM(ipv6) ];
            socket_address *addr;

            void setup( const socket_address &ip ) throw();
            void setup( const string &xname, const ip_version version);

			Y_DISABLE_COPY_AND_ASSIGN(socket_addr_ex);
        };

    }

}

#endif

