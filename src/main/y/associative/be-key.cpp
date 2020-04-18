
#include "y/associative/be-key.hpp"

namespace upsylon {
    
    be_key:: ~be_key() throw() {}
    
    be_key:: be_key( const be_key &k ) throw() :
    memory::ro_buffer(),
    address(k.address)
    {
    }
    
    size_t be_key:: length() const throw()
    {
        return BEaddress::size;
    }
    
    const void * be_key:: ro() const throw()
    {
        return & address.data;
    }
    

    
}

