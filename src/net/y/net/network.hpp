//! \file
#ifndef Y_NETWORK_INCLUDED
#define Y_NETWORK_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    namespace network
    {

        class net : public singleton<net>
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(net);
        };
    }
}

#endif

