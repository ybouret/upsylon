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
            
            double   wait_for() const throw();
            timeval *time_out() throw();


        private:
            timeval  usr; //!< set by usr
            timeval  org; //!< original value
            timeval *ptr; //!< &usr if delay>=0, NULL otherwise
            void initialize() throw();
            void copy_from( const socket_delay &other ) throw();
        };
    }

}

#endif

