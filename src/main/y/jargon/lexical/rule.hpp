//! \file

#ifndef Y_JARGON_LEXICAL_RULE_INCLUDED
#define Y_JARGON_LEXICAL_RULE_INCLUDED 1

#include "y/jargon/lexical/event/regular-family.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        namespace Lexical {
            
            //------------------------------------------------------------------
            //! base class for rules
            /**
             trigger and event uppon motif
             */
            //------------------------------------------------------------------

            class Rule : public Object, public inode<Rule>
            {
            public:
                
                const Tag           label; //!< shared label for Lexemes
                const Motif         motif; //!< motif to match
                const Event::Handle event; //!< event to trigger
                
                //! cleanup
                virtual ~Rule() throw();
                
                //! setup
                explicit Rule(const Tag           &ruleLabel,
                              const Motif         &ruleMotif,
                              const Event::Handle &ruleEvent);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };
            
            typedef core::list_of_cpp<Rule> Rules; //!< alias
            
        }
    }
}

#endif

