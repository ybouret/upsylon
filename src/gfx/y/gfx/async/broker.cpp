
#include "y/gfx/async/broker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {

            Broker:: ~Broker() throw()
            {
            }

            Broker:: Broker(const SharedEngine &eng,
                            const SharedServer &srv) throw()
            : engine(eng), server(srv)
            {

            }
            
        }
    }
}

