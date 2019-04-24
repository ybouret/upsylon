//! \file
#ifndef Y_NET_SOCKET_IO_PARAMS_INCLUDED
#define Y_NET_SOCKET_IO_PARAMS_INCLUDED 1

#include "y/net/types.hpp"

namespace upsylon
{
    namespace net
    {
        class socket_io_params
        {
        public:
            virtual ~socket_io_params() throw();
            explicit socket_io_params() throw();

            int send_flags;
            int recv_flags;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_io_params);
        };
    }
}

#endif

