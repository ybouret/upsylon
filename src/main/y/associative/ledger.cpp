#include "y/associative/ledger.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
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
    
    
    ledger:: ~ledger() throw()
    {
    }
    
    ledger:: ledger() : base_type()
    {
    }
    
    ledger:: ledger(const ledger &_) : base_type(_)
    {
    }
    
    ledger & ledger:: operator=( const ledger &_ )
    {
        { base_type &self = *this; self = _; }
        return *this;
    }


}
