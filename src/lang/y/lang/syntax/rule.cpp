
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

            Rule:: Rule(const uint32_t t) throw() : uuid(t), next(0), prev(0)
            {
            }
            
        }
    }
}

