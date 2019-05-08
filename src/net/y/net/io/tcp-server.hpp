
//! \file
#ifndef Y_NET_IO_TCP_SERVER_INCLUDED
#define Y_NET_IO_TCP_SERVER_INCLUDED 1

#include "y/net/tcp/server.hpp"
#include "y/net/socket/id-hasher.hpp"

#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace net
    {

        class tcp_client_protocol; //!< forward declaration

        //! protocol description
        class tcp_server_protocol : public tcp_server
        {
        public:
            virtual ~tcp_server_protocol() throw();                  //!< destructor
            explicit tcp_server_protocol(const socket_address &ip);  //!< default constructor
            virtual  tcp_client_protocol *create( const tcp_link &link );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_server_protocol);
        };

        typedef intr_ptr<socket_id_t,tcp_server_protocol> tcp_server_proto;    //!< smart pointer
        typedef memory::pooled                            tcp_server_alloc;    //!< allocator
        typedef set<socket_id_t,tcp_server_proto,                              //|
        socket_id_hasher,tcp_server_alloc>                tcp_server_db;       //!< database of tcp servers
        typedef tcp_server_db::iterator                   tcp_server_iterator; //!< tcp server iterator
    }

}

#endif

