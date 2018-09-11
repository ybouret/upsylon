
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


            Rule:: Rule(const Rule &other):
            Object(),
            next(0),
            prev(0),
            label(other.label),
            motif(other.motif),
            action(other.action)
            {
            }

            Rule:: Rule(const Origin &o, const Pattern::Handle &m, const Action &a ) :
            Object(),
            next(0),
            prev(0),
            label(o),
            motif(m),
            action(a)
            {

            }
            
        }
    }
}
