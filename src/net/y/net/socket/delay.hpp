//! \file
#ifndef Y_NET_SOCKET_DELAY_INCLUDED
#define Y_NET_SOCKET_DELAY_INCLUDED 1

#include "y/net/types.hpp"

namespace upsylon
{

    namespace net
    {
        class socket_delay
        {
        public:
            socket_delay( const double nsec=-1) throw();
            socket_delay(const socket_delay &) throw();
            socket_delay &operator=(const socket_delay &other) throw();
            
            ~socket_delay() throw();

            
            const struct timeval  usr; //!< set by usr
            const struct timeval  sys; //!< passed to select(...)
        private:
            struct timeval *ptr; //!< sys if delay>=0
            void initialize() throw();
            void copy_from( const socket_delay &other ) throw();
        };
    }

}

#endif

