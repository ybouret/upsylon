
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


            const char CXX_Comment:: init[] = "//";
        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            const char C_Comment:: init[] = "/\\*";
            const char C_Comment:: quit[] = "\\*/";

            C_Comment:: C_Comment(const string &id) : Comment(id,init)
            {
                {
                    const string drop_id = *label + ".drop";
                    const string drop_rx = ".";
                    drop(drop_id,drop_rx);
                }

                {
                    const string endl_id = *label + ".endl";
                    const string endl_rx = "[:endl:]";
                    endl(endl_id,endl_rx);
                }

                ret(quit);

            }
            
          

        }

    }

}

