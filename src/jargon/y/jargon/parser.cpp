
#include "y/jargon/parser.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Parser:: ~Parser() throw()
        {
        }
        
        XNode * Parser:: parse(Source &source)
        {
            return accept(*this,source);
        }
    }
}


