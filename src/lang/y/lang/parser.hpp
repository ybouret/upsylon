//! \file
#ifndef Y_LANG_PARSER_INCLUDED
#define Y_LANG_PARSER_INCLUDED 1

#include "y/lang/lexical/translator.hpp"
#include "y/lang/syntax/grammar.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Parser : public Lexical::Translator, public Syntax::Grammar
        {
        public:
            explicit Parser(const string &id);
            virtual ~Parser() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }
}

#endif


