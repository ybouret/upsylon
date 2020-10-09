
//! \file

#ifndef Y_JIVE_PARSER_INCLUDED
#define Y_JIVE_PARSER_INCLUDED 1


#include "y/jive/syntax/grammar.hpp"

namespace upsylon
{

    namespace Jive
    {
        class Parser : public Syntax::Grammar, public Lexer
        {
        public:
            virtual ~Parser() throw();

            template <typename ID> inline
            explicit Parser(const ID &id) :
            Syntax::Grammar(id),
            Lexer(name)
            {
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }

}

#endif

