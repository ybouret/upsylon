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

            class Comment : public Plugin
            {
            public:
                inline virtual ~Comment() throw() {}

                //! do nothing on Init
                inline virtual void Init(const Token &) {}

            protected:
                inline explicit Comment(const string &id, const char   *rx) : Plugin(id,rx) {}
                inline explicit Comment(const string &id, const string &rx) : Plugin(id,rx) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Comment);
            };

            class EndOfLineComment : public Comment
            {
            public:
                inline virtual ~EndOfLineComment() throw() {}
                inline explicit EndOfLineComment(const string &id, const char   *rx) : Comment(id,rx) { setup(); }
                inline explicit EndOfLineComment(const string &id, const string &rx) : Comment(id,rx) { setup(); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLineComment);
                void setup();
            };


            class CXX_Comment : public EndOfLineComment
            {
            public:
                inline virtual ~CXX_Comment() throw() {}
                inline explicit CXX_Comment(const string &id) : EndOfLineComment(id,init) {}

            private:
                static const char init[];
                Y_DISABLE_COPY_AND_ASSIGN(CXX_Comment);
            };

            class C_Comment : public Comment
            {
            public:
                inline
                virtual ~C_Comment() throw() {}       //!< desctructor
                explicit C_Comment(const string &id); //!< initialize
                
            private:
                static const char init[];
                static const char quit[];
            };
            

        }

    }

}

#endif

