
#include "y/net/socket/delay.hpp"
#include <cmath>

namespace upsylon
{
    namespace net
    {

        void socket_delay:: initialize() throw()
        {
            memset( (void*)&usr, 0, sizeof(usr) );
            memset( (void*)&sys, 0, sizeof(sys) );
        }


        socket_delay:: ~socket_delay() throw()
        {
            initialize(); ptr=0;
        }

        socket_delay:: socket_delay( const double nsec ) throw() :
        usr(),
        sys(),
        ptr(0)
        {
            initialize();
            if(nsec<0)
            {
                // do nothing
                ( (struct timeval *) &usr )->tv_sec = -1;
            }
            else
            {
                ptr = (struct timeval *)&sys;
                if(nsec>0)
                {
                    const double nsec_floor           = floor(nsec);
                    ((struct timeval *)&usr)->tv_sec  = int(nsec_floor);
                    ((struct timeval *)&usr)->tv_usec = int( floor( (nsec-nsec_floor)*1e6 + 0.5 ) );
                }
            }
        }


        void socket_delay:: copy_from(const socket_delay &other) throw()
        {
            assert( this != &other );
            memcpy( (void*)&usr, &other.usr, sizeof(usr) );
            memcpy( (void*)&sys, &other.sys, sizeof(sys) );
            if(other.ptr)
            {
                ptr = (struct timeval *)&sys;
            }
            else
            {
                ptr = NULL;
            }
        }

        socket_delay:: socket_delay( const socket_delay &other ) throw() :
        usr(),
        sys(),
        ptr(0)
        {
            copy_from(other);
        }

        socket_delay & socket_delay:: operator=( const socket_delay &other ) throw()
        {
            if(this!=&other)
            {
                copy_from(other);
            }
            return *this;
        }

    }
}
