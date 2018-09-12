
#include "y/lang/lexical/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Rule:: ~Rule() throw()
            {
            }


            Rule:: Rule(const Origin      &ruleLabel,
                        const Motif       &ruleMotif,
                        const RegularCode &ruleEvent) :
            label(ruleLabel),
            motif(ruleMotif),
            handle( & *ruleEvent ),
            event( (Event*)handle )
            {

            }

            Rule:: Rule(const Origin      &ruleLabel,
                        const Motif       &ruleMotif,
                        const ControlCode &ruleEvent) :
            label(ruleLabel),
            motif(ruleMotif),
            handle( & *ruleEvent ),
            event( (Event*)handle )
            {

            }

        }
    }
}
