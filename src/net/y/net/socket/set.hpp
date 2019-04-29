//! \file
#ifndef Y_NET_SOCKET_SET_INCLUDED
#define Y_NET_SOCKET_SET_INCLUDED 1

#include "y/net/socket/bsd.hpp"

namespace upsylon
{
    namespace net
    {
        //! wrapper for socket_set
        class socket_set : public net_object
        {
        public:
            static const size_t MAX_SIZE = FD_SETSIZE;
            explicit socket_set() throw();
            virtual ~socket_set() throw();

        private:
            fd_set fds; //!< system level desritptor
        };

    }
}

#endif

