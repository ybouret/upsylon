//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/socket/hasher.hpp"
#include "y/net/io/tcp-server.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace net
    {
        typedef set<socket_type,tcp_server_proto,socket_hasher> tcp_server_proto_db;
        typedef tcp_server_proto_db::iterator                   tcp_server_iterator;
        
        class io_engine : public net_object
        {
        public:
            explicit io_engine();
            virtual ~io_engine() throw();

            void start( tcp_server_protocol *srv );

            bool cycle( socket_delay &d );



        private:
            socket_set          sockset;
            tcp_server_proto_db tcp_servers;

            Y_DISABLE_COPY_AND_ASSIGN(io_engine);
        };
    }
}

#endif

