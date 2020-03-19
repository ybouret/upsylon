
#include "y/jargon/pattern/basic/any1.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
 
        Y_PATTERN_CLID(Any1);

        Any1:: Any1() throw() : Basic(UUID)
        {
            Y_PATTERN_SELF(Any1);
        }
        
        Any1:: ~Any1() throw()
        {
        }
        
        Pattern * Any1:: Create()
        {
            return new Any1();
        }
        
        Pattern * Any1:: clone() const
        {
            return new Any1();
        }
        
        size_t Any1::serialize(ios::ostream &fp) const
        {
            return emitUUID(fp);
        }
        
        bool Any1:: alike(const Pattern *p) const throw()
        {
            assert(p!=NULL);
            return UUID == p->uuid;
        }
        
        bool Any1:: good(const uint8_t) const throw()
        {
            return true;
        }

    }
    
}
