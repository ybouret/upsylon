
#include "y/jargon/axiom.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        template <typename T> static inline
        size_t  emitUUID(ios::ostream &fp, const T uuid)
        {
            size_t ans = 0 ;
            fp.emit_net(uuid,&ans);
            return ans;
        }
        
       
        
        size_t XNode:: serialize(ios::ostream &fp) const
        {
            switch(genre)
            {
                case IsInactive:
                    return emitUUID(fp,InactiveMark) + dogma->label->serialize(fp);
                    
                case IsTerminal:
                    break;
                    
                case IsInternal:
                    break;
            }
            return 0;
        }
        
        const char * XNode:: className() const throw()
        {
            return "XNode";
        }
        
    }
}
    
