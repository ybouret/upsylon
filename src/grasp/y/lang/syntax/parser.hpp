//! \file
#ifndef Y_LANG_SYNTAX_PARSER_INCLUDED
#define Y_LANG_SYNTAX_PARSER_INCLUDED 1

#include "y/lang/lexical/translator.hpp"
#include "y/lang/syntax/grammar.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Parser : public Lexer, public Grammar, public CountedObject
            {
            public:
                typedef intr_ptr<string,Parser> Pointer;

                virtual ~Parser() throw();          //!< destructor
                explicit Parser(const string &id);  //!< setup with *label = *name
                explicit Parser(const char   *id);  //!< setup with *label = *name
                const string & key() const throw(); //!< *name

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }
}

#endif

