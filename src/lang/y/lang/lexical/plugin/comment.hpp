
#ifndef Y_LANG_LEXICAL_COMMENT_INCLUDED
#define Y_LANG_LEXICAL_COMMENT_INCLUDED 1

#include "y/lang/lexical/scanner.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            class EndOfLine : public Plugin
            {
            public:
                explicit EndOfLine(const string &id,const string &rx);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };

        }

    }

}

#enfif

