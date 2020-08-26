
#include "y/jive/context.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace Jive
    {

        Context:: ~Context() throw()
        {
            _bzset(line);
            _bzset(column);

        }

        Context:: Context(const Context &other) throw() :
        Tag(other),
        line(other.line),
        column(other.column)
        {
        }
        
    }

}


