
#include "y/jargon/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        Context:: ~Context() throw()
        {
            _bzset(line);
            _bzset(column);
        }
        
#define Y_JCONTEXT_INI() line(1), column(1)
#define Y_JCONTEXT(type) \
Context:: Context(type dummy) : tag( new string(dummy) ), Y_JCONTEXT_INI(){}
        
        
        Context:: Context(const Tag &id) throw() : tag(id), Y_JCONTEXT_INI() {}
        
        Y_JCONTEXT(const char   *)
        Y_JCONTEXT(const string &)

        Context:: Context(const Context &other) throw() :
        tag(    other.tag    ),
        line(   other.line   ),
        column( other.column )
        {
        }
        
        
        
        
    }
    
}
