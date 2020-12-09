
//! \file

#ifndef Y_NET_COMM_TCP_SERVER_INCLUDED
#define Y_NET_COMM_TCP_SERVER_INCLUDED 1

#include "y/net/tcp/server.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"
#include "y/net/socket/id-hasher.hpp"

namespace upsylon
{
    namespace net
    {
        class comm_tcp_client; //!< forward declaration

        class comm_tcp_server : public tcp_server
        {
        public:
            virtual ~comm_tcp_server() throw();                       //!< destructor
            explicit comm_tcp_server(const socket_address &ip);       //!< default constructor
            virtual  comm_tcp_client *create( const tcp_link &link ); //!< called upon accepted tcp connection

            typedef intr_ptr<socket_id_t,comm_tcp_server>         proto;    //!< smart pointer
            typedef hash_set<socket_id_t,proto,socket_id_hasher>  db;       //!< database of tcp servers
            typedef db::iterator                                  iterator; //!< tcp server iterator

        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_tcp_server);
        };
    }

}

#endif
