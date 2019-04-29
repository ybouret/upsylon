
#include "y/net/socket/set.hpp"

#include <sys/select.h>

namespace upsylon
{
    namespace net
    {
        socket_set:: ~socket_set() throw()
        {
        }

        socket_set:: socket_set() throw() :
        fds()
        {
            FD_ZERO(&fds);
        }

    }
}
