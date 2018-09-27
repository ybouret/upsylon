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


            //! CXX comment prototype
            class CXX_Comment : public EndOfLineComment
            {
            public:
                inline virtual ~CXX_Comment() throw() {} //!< desctructor
                inline explicit CXX_Comment(Translator &t, const string &id) : EndOfLineComment(t,id,init) {} //!< initialzie

            private:
                static const char init[];
                Y_DISABLE_COPY_AND_ASSIGN(CXX_Comment);
            };

            //! Multiple Lines Comment
            class MultiLinesComment : public Comment
            {
            public:
                const string closing; //!< the closing pattern expression

                //! destructor
                inline virtual ~MultiLinesComment() throw() {}

                //! initialize
                inline explicit MultiLinesComment(Translator &t, const string &id, const char   *rx_ini, const char   *rx_end) : Comment(t,id,rx_ini), closing(rx_end) { fill(); }

                //! initialize
                inline explicit MultiLinesComment(Translator &t, const string &id, const string &rx_ini, const string &rx_end) : Comment(t,id,rx_ini), closing(rx_end) { fill(); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComment);
                void fill();
            };

            //! C multilines comment
            class C_Comment : public MultiLinesComment
            {
            public:
                //! destructor
                inline virtual ~C_Comment() throw() {}
                
                //! initialize
                inline explicit C_Comment(Translator &t, const string &id) : MultiLinesComment(t,id,init,quit) {}
                
            private:
                static const char init[];
                static const char quit[];
            };
            

        }

    }

}

#endif

