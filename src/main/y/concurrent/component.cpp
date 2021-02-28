
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
        
    }
}

