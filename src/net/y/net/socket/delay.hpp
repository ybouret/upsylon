//! \file
#ifndef Y_NET_SOCKET_DELAY_INCLUDED
#define Y_NET_SOCKET_DELAY_INCLUDED 1

#include "y/net/types.hpp"

namespace upsylon
{

    namespace net
    {
        //! wrapper to use time in select(...)
        class socket_delay
        {
        public:
            socket_delay( const double nsec=-1) throw();           //!< setup
            socket_delay(const socket_delay &) throw();            //!< copy
            socket_delay &operator=(const socket_delay &) throw(); //!< assign
            ~socket_delay() throw();                               //!< cleanup
            
            double   wait_for() const throw(); //!< waiting tme
            timeval *time_out() throw();       //!< presentation for select(...)
            
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

