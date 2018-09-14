
#include "y/lang/lexical/plugin/jstring.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            jString:: ~jString() throw()
            {
            }

            static const char __quote[] = "\\x22";

            jString:: jString(const string &id) : Plugin(id,__quote), content()
            {
                
                ret(__quote);
            }

            void jString:: Init( const Token & )
            {
                content.clear();
            }


        }
    }
}


