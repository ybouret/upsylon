
#include "y/os/be-address.hpp"
#include "y/os/endian.hpp"

namespace upsylon {
    
    void * BEaddress:: BE( void *addr ) throw()
    {
        return swap_be_as<void*>(addr);
    }
    
    BEaddress:: ~BEaddress() throw()
    {
        
    }
    
    BEaddress:: BEaddress(const BEaddress &other ) throw() :
    data( other.data )
    {
    }
    
}

