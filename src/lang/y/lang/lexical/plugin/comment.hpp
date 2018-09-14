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

            class EndOfLineComment : public Plugin
            {
            public:
                inline virtual ~EndOfLineComment() throw() {}
                inline explicit EndOfLineComment(const string &id,const char *rx) : Plugin(id,rx) { setup(); }
                inline explicit EndOfLineComment(const char   *id,const char *rx) : Plugin(id,rx) { setup(); }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLineComment);
                void setup();
            };

            class CXX_Comment : public EndOfLineComment
            {
            public:
                inline virtual ~CXX_Comment() throw() {}
                inline CXX_Comment(const string &id ) : EndOfLineComment(id,rx) {}
                inline CXX_Comment(const char   *id ) : EndOfLineComment(id,rx) {}

            private:
                static const char rx[];
                Y_DISABLE_COPY_AND_ASSIGN(CXX_Comment);
            };

            

        }

    }

}

#endif

