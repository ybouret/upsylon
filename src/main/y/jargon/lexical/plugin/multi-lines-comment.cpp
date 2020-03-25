
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            MultiLinesComment:: ~MultiLinesComment() throw()
            {
            }
            
            void MultiLinesComment:: setup()
            {
                endl("endl","[:endl:]");
                drop("dot",".");
            }
            
            void MultiLinesComment:: onInit(const Token &) throw()
            {
                
            }
        }
        
    }
}
