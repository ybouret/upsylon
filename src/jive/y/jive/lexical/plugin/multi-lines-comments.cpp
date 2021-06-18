

#include "y/jive/lexical/plugin/multi-lines-comments.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            MultiLinesComments:: ~MultiLinesComments() throw()
            {
            }

            void MultiLinesComments:: setup()
            {
                drop("dot", ".");
                silent_endl("endl","[:endl:]");
            }

            void MultiLinesComments:: OnInit(const Token &)
            {
                
            }


        }

    }
}

