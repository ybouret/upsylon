
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            Y_JARGON_PLUGIN_IMPL(MultiLinesComment);

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
            
            void MultiLinesComment:: collectEOS(string &data) const
            {
                data = *trigger;
            }

            
        }
        
    }
}
