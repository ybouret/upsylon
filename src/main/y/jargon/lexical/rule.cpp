

#include "y/jargon/lexical/rule.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            Rule:: ~Rule() throw()
            {
            }
            
            Rule:: Rule(const Tag           &ruleLabel,
                        const Motif         &ruleMotif,
                        const Event::Handle &ruleEvent) :
            Object(), inode<Rule>(),
            label( ruleLabel ),
            motif( ruleMotif ),
            event( ruleEvent )
            {
                if( motif->feeble() )
                {
                    throw exception("Jargon::Rule <%s> has feeble motif", **ruleLabel);
                }
            }
            
        }
    }
}
