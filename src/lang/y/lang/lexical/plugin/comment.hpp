//! \file
#ifndef Y_LANG_LEXICAL_COMMENT_INCLUDED
#define Y_LANG_LEXICAL_COMMENT_INCLUDED 1

#include "y/lang/lexical/plugin.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            class EndOfLine : public Plugin
            {
            public:
                inline virtual ~EndOfLine() throw() {}
                inline explicit EndOfLine(const string &id,const char *rx) : Plugin(id,rx) { setup(); }
                inline explicit EndOfLine(const char   *id,const char *rx) : Plugin(id,rx) { setup(); }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLine);
                void setup();
            };

            


        }

    }

}

#endif

