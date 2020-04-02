
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
            Parser( const LABEL &id ) :
            Lexer(id),
            Grammar(id),
            tcache()
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
            
            //! load a 0-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label)
            {
                load(type2type<PLUGIN>(),label).hook(*this);
                return terminal(label);
            }
            
            //! load a 1-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter)
            {
                load(type2type<PLUGIN>(),label,enter).hook(*this);
                return terminal(label);
            }
            
            template <typename PLUGIN,typename LABEL,typename ENTER,typename LEAVE>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter, const LEAVE &leave)
            {
                load(type2type<PLUGIN>(),label,enter,leave).hook(*this);
                return terminal(label);
            }
            
            mutable Cache tcache;
            
            XNode *parse( Source &source )
            {
                return accept(*this,source);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
        
    }
    
}
#endif
