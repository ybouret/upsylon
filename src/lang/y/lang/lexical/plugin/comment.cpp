
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            void EndOfLineComment:: setup()
            {
                {
                    const string drop_id = *label + ".drop";
                    const string drop_rx = ".";
                    drop(drop_id,drop_rx);
                }

                back("[:endl:]", this, & Scanner::newline );
            }


            const char CXX_Comment:: rx[] = "//";
        }
    }
}

