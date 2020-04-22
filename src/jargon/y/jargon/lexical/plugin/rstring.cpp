

#include "y/jargon/lexical/plugin/rstring.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            Y_JARGON_PLUGIN_IMPL(rString);

            rString:: ~rString() throw()
            {
            }
            
            const char rString::Separator = '\'';
        }
        
    }
    
}

