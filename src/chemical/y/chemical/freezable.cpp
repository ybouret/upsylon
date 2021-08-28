

#include "y/chemical/freezable.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Freezable:: ~Freezable() throw() {}
        
        Freezable:: Freezable() throw() : frozen(false)
        {
        }
        
        Freezable:: Latch:: ~Latch() throw()
        {
            aliasing::_(host.frozen) = false;
        }
        
        Freezable:: Latch:: Latch(Freezable &f) throw() :
        host(f)
        {
            assert(host.frozen==false);
            aliasing::_(host.frozen) = true;
        }
    }
    
    
}
