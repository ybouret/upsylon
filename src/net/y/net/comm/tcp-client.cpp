
#include "y/net/comm/tcp-client.hpp"

namespace upsylon
{
    namespace net
    {
        comm_tcp_client:: ~comm_tcp_client() throw()
        {
        }

        comm_tcp_client::  comm_tcp_client(const tcp_link &given_connexion,
                                           const size_t    send_block_size,
                                           const size_t    recv_block_size):
        net_object(),
        comm_queues(send_block_size,recv_block_size),
        link(given_connexion)
        {
        }

        const socket_id_t & comm_tcp_client:: key() const throw()
        {
            return link->key();
        }

        const bsd_socket  & comm_tcp_client:: operator*() const throw()
        {
            return *link;
        }

    }

}


