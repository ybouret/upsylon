
#include "y/net/tcp/stream.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_stream:: ~tcp_stream() throw()
        {

        }

        tcp_stream:: tcp_stream( const tcp_link &conn ) throw() :
        link(conn)
        {
        }
        
    }

}





