//! \file
#ifndef Y_LANG_PARSER_INCLUDED
#define Y_LANG_PARSER_INCLUDED 1

#include "y/lang/syntax/grammar.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            class Parser : public Lexical::Translator, public Grammar
            {
            public:
                explicit Parser(const string &id);
                virtual ~Parser() throw();

                //! create a new terminal
                Rule & __term(const string &id, const string &rx, const Terminal::Attribute attr);

                //! a regular terminal
                inline
                Rule & term( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Standard);
                }

                //! a regular terminal
                inline
                Rule & term(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return term(_,__);
                }

                //! an univocal=alias terminal
                inline
                Rule & alias( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Univocal);
                }

                //! an univocal=alias terminal
                inline
                Rule & alias(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return alias(_,__);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };
        }
    }
}

#endif


