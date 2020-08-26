
#include "y/jive/context.hpp"

namespace upsylon {

    namespace Jive
    {

        Context:: ~Context() throw()
        {
        }

        Context:: Context(const Context &other) throw() :
        Tag(other),
        line(other.line),
        column(other.column)
        {
        }
        
    }

}


