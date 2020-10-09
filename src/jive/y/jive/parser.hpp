
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
            typedef Syntax::Axiom Axiom;

            virtual ~Parser() throw();


            template <typename ID> inline
            explicit Parser(const ID &id) :
            Syntax::Grammar(id),
            Lexer(name)
            {
            }


            template <typename ID,typename RX>
            const Axiom & terminal( const ID &id, const RX &rx )
            {
                emit(id,rx);
                return term(id);
            }

            void   start() throw();
            XNode *parse(Source &source,const size_t prefetch=0);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }

}

#endif

