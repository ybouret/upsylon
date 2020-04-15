#include "y/associative/ledger.hpp"

namespace upsylon
{
   
    
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
