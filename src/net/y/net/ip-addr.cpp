
#include "y/net/ip-addr.hpp"
#include "y/memory/io.hpp"

namespace upsylon
{
    namespace net
    {

        ip_address:: ~ip_address() throw()
        {
        }

        ip_address:: ip_address( void *data, const unsigned port_offset ) throw() :
        port( *memory::io::cast<net16_t>(data,port_offset) )
        {
        }
        

        ip_data<v4>::~ip_data() throw()
        {
            memset( &sa, 0, sizeof(sa) );
        }

        ip_data<v4>:: ip_data() throw() : sa()
        {
            memset( &sa, 0, sizeof(sa) );
            sa.sin_family = AF_INET;
        }


        ip_data<v6>::~ip_data() throw()
        {
            memset( &sa, 0, sizeof(sa) );
        }

        ip_data<v6>:: ip_data() throw() : sa()
        {
            memset( &sa, 0, sizeof(sa) );
            sa.sin6_family = AF_INET;
        }

    }
}
