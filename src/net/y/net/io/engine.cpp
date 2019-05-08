#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        engine:: ~engine() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.engine.quit]" << std::endl);
        }

        engine:: engine() :
        sockset()
        {
            Y_NET_VERBOSE(std::cerr << "[network.engine.init]" << std::endl);
        }

        
        

    }
}
