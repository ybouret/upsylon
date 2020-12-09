
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

        //______________________________________________________________________
        //
        //
        //! dynamic comm tcp server
        //
        //______________________________________________________________________
        class comm_tcp_server : public tcp_server
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<socket_id_t,comm_tcp_server>         proto;    //!< smart pointer
            typedef hash_set<socket_id_t,proto,socket_id_hasher>  db;       //!< database of tcp servers
            typedef db::iterator                                  iterator; //!< tcp server iterator

            //__________________________________________________________________
            //
            //C++
            //__________________________________________________________________
            virtual ~comm_tcp_server() throw();                       //!< destructor
            explicit comm_tcp_server(const socket_address &ip);       //!< default constructor

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual  comm_tcp_client *create( const tcp_link &link ); //!< called upon accepted tcp connection


        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_tcp_server);
        };
    }

}

#endif
