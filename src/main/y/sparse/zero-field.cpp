#include "y/sparse/zero-field.hpp"
#include "y/object.hpp"
#include <cstring>

namespace upsylon
{
    namespace sparse
    {
        zero_field_:: ~zero_field_() throw()
        {
            memset(item_,0,itsz_);
            object:: operator delete(item_,itsz_);
            item_ = 0;
            itsz_ = 0;
        }

        zero_field_:: zero_field_( const size_t item_size ) :
        itsz_(item_size),
        item_( object::operator new(itsz_) )
        {

        }
    }
}
