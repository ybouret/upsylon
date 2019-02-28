#include "y/lang/syntax/grammar.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() throw()
            {
            }

            Grammar:: Grammar(const Tag &id) :
            Object(),
            name(id),
            rules(),
            verbose(false)
            {
            }

            void Grammar:: setVerbose(const bool flag) throw()
            {
                verbose = flag;
                for(Rule *r=rules.head;r;r=r->next)
                {
                    r->verbose = flag;
                }
            }
        }
    }
}
