
#include "y/collection.hpp"
#include "y/type/int2int.hpp"

namespace upsylon {

    collection:: collection() throw()
    {
        
    }

    collection:: ~collection() throw()
    {

    }

    unit_t collection:: ssize() const
    {
        return i2i<unit_t,size_t>( size() );
    }
}
