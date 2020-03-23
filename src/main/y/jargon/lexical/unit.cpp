
#include "y/jargon/lexical/unit.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
          
            Unit:: ~Unit() throw()
            {
            }
            
            Unit:: Unit(const Cache   &cache,
                        const Context &ctx,
                        const Tag     &t) throw() :
            Object(),
            Token(cache),
            Context(ctx),
            inode<Unit>(),
            label(t)
            {
                
            }
            
        }
        
    }
    
}
