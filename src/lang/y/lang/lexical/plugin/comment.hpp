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

            //! Comment base class
            class Comment : public Plugin
            {
            public:
                inline virtual ~Comment() throw() {}       //!< desctructor
                inline virtual void Init(const Token &) {} //!< do nothing on Init

            protected:
                inline explicit Comment(Translator &t, const string &id, const char   *rx) : Plugin(t,id,rx) {} //!< initialize
                inline explicit Comment(Translator &t, const string &id, const string &rx) : Plugin(t,id,rx) {} //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Comment);
            };

            //! End of line comment
            class EndOfLineComment : public Comment
            {
            public:
                inline virtual ~EndOfLineComment() throw() {} //!< desctructor
                inline explicit EndOfLineComment(Translator &t, const string &id, const char   *rx) : Comment(t,id,rx) { setup(); } //!< initialize
                inline explicit EndOfLineComment(Translator &t, const string &id, const string &rx) : Comment(t,id,rx) { setup(); } //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLineComment);
                void setup();
            };


            //! CXX comment prototup
            class CXX_Comment : public EndOfLineComment
            {
            public:
                inline virtual ~CXX_Comment() throw() {} //!< desctructor
                inline explicit CXX_Comment(Translator &t, const string &id) : EndOfLineComment(t,id,init) {} //!< initialzie

            private:
                static const char init[];
                Y_DISABLE_COPY_AND_ASSIGN(CXX_Comment);
            };

            //! C multilines comment
            class C_Comment : public Comment
            {
            public:
                inline
                virtual ~C_Comment() throw() {}                      //!< destructor
                explicit C_Comment(Translator &t, const string &id); //!< initialize
                
            private:
                static const char init[];
                static const char quit[];
            };
            

        }

    }

}

#endif

