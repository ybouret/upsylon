
#include "y/jargon/lexical/plugin/jstring.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            Y_JARGON_PLUGIN_IMPL(jString);

            jString:: ~jString() throw()
            {
            }
            
            const char jString::Separator = '"';
        }
        
    }
    
}

