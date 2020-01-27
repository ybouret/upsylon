
#include "y/type/mark.hpp"

namespace upsylon {

    type_mark:: type_mark( const type_mark &other ) throw() :
    type_mark_(other)
    {
    }

    type_mark & type_mark:: operator=(const type_mark &other) throw()
    {
        type_mark_ &self = *this;
        self=other;
        return *this;
    }

    type_mark:: ~type_mark() throw()
    {
    }

    type_mark:: type_mark( const type_spec &ts ) throw() : type_mark_( &ts ) {}

    
}
