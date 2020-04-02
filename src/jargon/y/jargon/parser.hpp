
#ifndef Y_JARGON_PARSER_INCLUDED
#define Y_JARGON_PARSER_INCLUDED 1

#include "y/jargon/grammar.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Parser : public Lexer, public Grammar
        {
        public:
            virtual ~Parser() throw();
            
            template <typename LABEL> inline
            Parser( const LABEL &id ) : Lexer(id), Grammar(id)
            {
            }
            
            template <typename LABEL,typename REGEXP>
            Axiom & term(const LABEL &label, const REGEXP &regexp )
            {
                // will emit the terminal
                emit(label,regexp);
                
                // and associate it with a terminal
                return terminal(label);
            }
            
            template <typename LABEL>
            Axiom & term(const LABEL &both)
            {
                return term(both,both);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
        
    }
    
}
#endif
