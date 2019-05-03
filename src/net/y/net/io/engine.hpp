//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/io/tcp-server.hpp"

#include "y/associative/set.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace net
    {
        typedef memory::pooled                                   io_memory; //!< internal memory model
        typedef network::hasher                                  io_hasher; //!< copy socket hkey

        typedef set<size_t,tcp_server_proto,io_hasher,io_memory> tcp_server_proto_db; //!< alias
        typedef tcp_server_proto_db::iterator                    tcp_server_iterator; //!< alias

        typedef set<size_t,tcp_client_proto,io_hasher,io_memory> tcp_client_proto_db; //!< alias
        typedef tcp_client_proto_db::iterator                    tcp_client_iterator; //!< alias


        //! handle connections
        class io_engine : public net_object
        {
        public:
            static const size_t tcp_servers_init = 2; //!< default capacity
            static const size_t tcp_clients_init = 2; //!< default capacity

            explicit io_engine();         //!< setup
            virtual ~io_engine() throw(); //!< cleanup

            //! register a (taken care of) tcp_server_protocol
            void start( tcp_server_protocol *srv );

            //! register a (taken care of) tcp_client_protocol
            void start( tcp_client_protocol *cln );

            //! perform a cycle, return true if an activity is detected before timeout
            bool cycle( socket_delay &d );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_engine);

            socket_set          sockset;
            tcp_server_proto_db tcp_servers;
            tcp_client_proto_db tcp_clients;

            void check_tcp_servers( size_t &na );
            void check_tcp_clients( size_t &na );
        };
    }
}

#endif

