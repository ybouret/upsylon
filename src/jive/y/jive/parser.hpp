
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
            typedef Syntax::Axiom     Axiom;
            typedef Syntax::Terminal  Terminal;
            typedef Syntax::Aggregate Aggregate;
            typedef Syntax::Alternate Alternate;
            
            virtual ~Parser() throw();
            
            
            template <typename ID> inline
            explicit Parser(const ID &id) :
            Syntax::Grammar(id),
            Lexer(name)
            {
            }
            
            
            template <typename ID,typename RX> inline
            const Terminal & terminal(const ID &id,
                                      const RX &rx)
            {
                const Lexical::Rule &rule = emit(id,rx);
                return term(id,rule.motif->univocal() ? Terminal::Univocal : Terminal::Standard );
            }
            
            
            template <typename ID> inline
            const Terminal & terminal(const ID &id)
            {
                return terminal(id,id);
            }
            
            template <typename ID,typename RX> inline
            const Terminal & division(const ID &id,
                                      const RX &rx)
            {
                (void) emit(id,rx);
                return term(id,Terminal::Division);
            }
            
            template <typename ID> inline
            const Terminal & division(const ID &id)
            {
                return division(id,id);
            }
            
            template <typename PLUGIN,typename ID>
            const Terminal & plugin(const ID &id)
            {
                const Lexical::Rule &r = call( plug<PLUGIN>(id) );
                return term(r.label,Terminal::Standard);
            }
            
            
            
            void   start() throw();
            XNode *parse(Source &source,const size_t prefetch=0);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }
    
}

#endif

