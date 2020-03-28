
#include "y/jargon/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/jargon/tags.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        Context:: ~Context() throw()
        {
            _bzset(line);
            _bzset(column);
        }
        
#define Y_JCONTEXT(type) \
Context:: Context(type dummy) : tag( Tags::Make(dummy) ), line(1), column(1) {}
        
        
        Y_JCONTEXT(const char   *)
        Y_JCONTEXT(const string &)
        Y_JCONTEXT(const Tag    &)

        Context:: Context(const Context &other) throw() :
        tag(    other.tag    ),
        line(   other.line   ),
        column( other.column )
        {
        }
        
        
        
        
    }
    
}
