#include "y/container/const-field.hpp"
#include "y/object.hpp"
#include "y/type/block/zset.hpp"
#include <cstring>

namespace upsylon
{
    const_field_:: ~const_field_() throw()
    {
        memset(item_addr,0,item_size);
        object:: operator delete(item_addr,item_size);
        item_addr          = 0;
        _bzset(item_size);
    }

    const_field_:: const_field_( const size_t bytes ) :
    item_size(bytes),
    item_addr( object::operator new(item_size) )
    {

    }
}
