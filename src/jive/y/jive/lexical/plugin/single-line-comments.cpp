
#include "y/jive/lexical/plugin/single-line-comments.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            SingleLineComments:: ~SingleLineComments() throw()
            {
            }


            void SingleLineComments:: OnInit(const Token &)
            {
                
            }

            void SingleLineComments:: setup()
            {
                back("[:endl:]",this, & Scanner::newLine);
                drop("dot",".");
            }

        }

    }
}

