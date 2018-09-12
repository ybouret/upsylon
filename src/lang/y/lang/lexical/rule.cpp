
#include "y/lang/lexical/rule.hpp"
#include "y/exception.hpp"

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
                checkMotif();
            }

            Rule:: Rule(const Origin      &ruleLabel,
                        const Motif       &ruleMotif,
                        const ControlCode &ruleEvent) :
            label(ruleLabel),
            motif(ruleMotif),
            handle( & *ruleEvent ),
            event( (Event*)handle )
            {
                checkMotif();
            }


            void Rule:: checkMotif() const
            {
                if(motif->weak())
                {
                    const char *rid = **label;
                    const char *tid = "Regular";
                    if(event->type==Event::Control)
                    {
                        tid = "Control";
                    }
                    throw exception("Lexical::Rule <%s>/%s : Weak Pattern", rid,tid);
                }
            }
        }
    }
}
