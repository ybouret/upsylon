
#include "y/jargon/pattern.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
            self = 0;
        }
        
        Pattern:: Pattern(const uint32_t u) throw() :
        uuid(u),
        self(0)
        {
        }
        
    }
    
}
