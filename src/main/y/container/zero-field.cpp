#include "y/container/zero-field.hpp"
#include "y/object.hpp"
#include <cstring>

namespace upsylon
{
    zero_field_:: ~zero_field_() throw()
    {
        memset(item_addr,0,item_size);
        object:: operator delete(item_addr,item_size);
        item_addr          = 0;
        (size_t&)item_size = 0;
    }

    zero_field_:: zero_field_( const size_t bytes ) :
    item_size(bytes),
    item_addr( object::operator new(item_size) )
    {

    }
}
