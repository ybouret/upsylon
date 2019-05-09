//! \file
#ifndef Y_NET_IO_TCP_CLIENT_INCLUDED
#define Y_NET_IO_TCP_CLIENT_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/net/socket/id-hasher.hpp"
#include "y/net/io/tcp-queue.hpp"

#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace net
    {

        //! protocol to describe interactions
        class tcp_client_protocol : public net_object
        {
        public:
            virtual ~tcp_client_protocol() throw();                  //!< destructor
            explicit tcp_client_protocol(const tcp_link &l) throw(); //!< default constructor
            
            const socket_id_t & key() const throw();      //!< inner socket uuid
            const bsd_socket  & operator*()const throw(); //!< inner bsd_socket for socket_set ops

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_client_protocol);
            tcp_link link;
        };

        typedef intr_ptr<socket_id_t,tcp_client_protocol> tcp_client_proto;    //!< smart pointer
        typedef memory::global                            tcp_client_alloc;    //!< memory allocator
        typedef set<socket_id_t,tcp_client_proto,                              //|
        socket_id_hasher,tcp_client_alloc>                tcp_client_db;       //!< database of tcp clients
        typedef tcp_client_db::iterator                   tcp_client_iterator; //!< tcp client iterator
    }

}

#endif

