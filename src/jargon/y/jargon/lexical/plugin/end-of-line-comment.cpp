
#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
     
            EndOfLineComment:: ~EndOfLineComment() throw()
            {
            }
            
            void EndOfLineComment:: setup()
            {
                back("[:endl:]", this, & Scanner::newLine);
                drop("dot",".");
            }
            
            void EndOfLineComment:: onInit(const Token &) throw()
            {
                
            }
            
            Y_JARGON_PLUGIN_IMPL(EndOfLineComment);
        }
        
    }
}
