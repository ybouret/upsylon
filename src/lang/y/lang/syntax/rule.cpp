
#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Rule:: ~Rule() throw()
            {
            }

            Rule:: Rule(const uint32_t t, const string &id) :
            uuid(t),
            name(id),
            next(0),
            prev(0)
            {
            }
            
        }
    }
}

