//! \file
#ifndef Y_NET_SOCKET_IO_PARAMS_INCLUDED
#define Y_NET_SOCKET_IO_PARAMS_INCLUDED 1

#include "y/net/types.hpp"

namespace upsylon
{
    namespace net
    {
        //! parameters for I/O
        class socket_io_params
        {
        public:
            virtual ~socket_io_params() throw(); //!< setup
            explicit socket_io_params() throw(); //!< cleanup

            int send_flags; //!< for send functions, default=0
            int recv_flags; //!< for recv functions, default=0


        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_io_params);
        };
    }
}

#endif

