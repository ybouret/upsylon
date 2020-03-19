
#include "y/jargon/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"

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
        
        size_t Pattern:: emitUUID(ios::ostream &fp) const
        {
            size_t nw = 0;
            fp.emit_net(uuid,&nw);
            return nw;
        }

    }
    
}
