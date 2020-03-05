
#include "y/net/socket/delay.hpp"
#include "y/type/block-zset.hpp"

#include <cmath>

namespace upsylon
{
    namespace net
    {

        void socket_delay:: initialize() throw()
        {
            bzset(usr);
            bzset(org);
        }


        socket_delay:: ~socket_delay() throw()
        {
            initialize(); ptr=0;
        }

        socket_delay:: socket_delay( const double nsec ) throw() :
        usr(),
        org(),
        ptr(0)
        {
            initialize();
            if(nsec<0)
            {
                // do nothing
                usr.tv_sec = -1;
            }
            else
            {
                ptr = (struct timeval *)&usr;
                if(nsec>0)
                {
                    const double nsec_floor           = floor(nsec);
                    usr.tv_sec  = int(nsec_floor);
                    usr.tv_usec = int( floor( (nsec-nsec_floor)*1e6 + 0.5 ) );
                }
            }
            memcpy( &org, &usr, sizeof(timeval) );
        }


        void socket_delay:: copy_from(const socket_delay &other) throw()
        {
            assert( this != &other );
            memcpy( &usr, &other.usr, sizeof(timeval) );
            memcpy( &org, &other.org, sizeof(timeval) );
            if(other.ptr)
            {
                ptr = (struct timeval *)&usr;
            }
            else
            {
                ptr = NULL;
            }
        }

        socket_delay:: socket_delay( const socket_delay &other ) throw() :
        usr(),
        org(),
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


        double socket_delay:: wait_for() const throw()
        {
            return double(usr.tv_sec) + 1e-6*double(usr.tv_usec);
        }

        struct timeval * socket_delay:: time_out()  throw()
        {
            memcpy(&usr,&org,sizeof(timeval));
            return ptr;
        }


    }
}
