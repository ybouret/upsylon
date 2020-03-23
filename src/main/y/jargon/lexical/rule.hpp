//! \file

#ifndef Y_JARGON_LEXICAL_RULE_INCLUDED
#define Y_JARGON_LEXICAL_RULE_INCLUDED 1

#include "y/jargon/lexical/event/regular-family.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        namespace Lexical {
            
            class Rule : public Object, public inode<Rule>
            {
            public:
                virtual ~Rule() throw();
                
                const Tag           label;
                const Motif         motif;
                const Event::Handle event;
                
                
                explicit Rule(const Tag           &ruleLabel,
                              const Motif         &ruleMotif,
                              const Event::Handle &ruleEvent);
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };
            
            typedef core::list_of_cpp<Rule> Rules;
            
        }
    }
}

#endif

