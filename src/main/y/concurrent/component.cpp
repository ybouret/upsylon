
#include "y/concurrent/component.hpp"

namespace upsylon
{
    namespace concurrent
    {
        component:: ~component() throw()
        {
        }
        
        component::component() : access(), topo( new topology() )
        {
        }

        component:: component(Y_CONCURRENT_TOPO_ARGS) :
        access(), topo( new topology(Y_CONCURRENT_TOPO_ARGS_) )
        {
        }
        

    }
}

