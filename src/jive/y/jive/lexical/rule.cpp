


#include "y/jive/lexical/rule.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            Rule:: ~Rule() throw()
            {
            }

            Rule:: Rule(const Tag           &ruleLabel,
                        const Motif         &ruleMotif,
                        const Event::Handle &ruleEvent) :
            Object(),
            dnode<Rule>(),
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
