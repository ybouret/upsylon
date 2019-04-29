//! \file
#ifndef Y_NET_SOCKET_SET_INCLUDED
#define Y_NET_SOCKET_SET_INCLUDED 1

#include "y/net/socket/bsd.hpp"

namespace upsylon
{
    namespace net
    {

        class socket_set : public net_object
        {
        public:
            explicit socket_set() throw();
            virtual ~socket_set() throw();
            
        private:
            fd_set fds; //!< system level desritptor
        };

    }
}

#endif

