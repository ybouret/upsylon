`//! \file

#ifndef Y_NET_COMM_TCP_CLIENT_INCLUDED
#define Y_NET_COMM_TCP_CLIENT_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/net/comm/queues.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"
#include "y/net/socket/id-hasher.hpp"

namespace upsylon
{
    namespace net
    {

        //______________________________________________________________________
        //
        //
        //! dynamic comm tcp client
        //
        //______________________________________________________________________
        class comm_tcp_client  : public net_object, public comm_queues
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<socket_id_t,comm_tcp_client>        proto;    //!< alias
            typedef hash_set<socket_id_t,proto,socket_id_hasher> db;       //!< alias
            typedef db::iterator                                 iterator; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_tcp_client(const tcp_link &given_connexion,
                                     const size_t    send_block_size,
                                     const size_t    recv_block_size);
            virtual ~comm_tcp_client() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const socket_id_t & key()       const throw(); //!< inner socket uuid
            const bsd_socket  & operator*() const throw(); //!< inner bsd_socket for socket_set ops



        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_tcp_client);
            tcp_link link;
        };

    }
}



#endif
`
