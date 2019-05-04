#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        io_engine:: ~io_engine() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.io_engine.quit]" << std::endl);
        }

        io_engine:: io_engine() :
        sockset()
        {
            Y_NET_VERBOSE(std::cerr << "[network.io_engine.init]" << std::endl);
        }

        
        

    }
}
