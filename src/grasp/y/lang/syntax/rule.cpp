#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace  Lang
    {

        namespace Syntax
        {
            Rule:: ~Rule() throw()
            {
            }

            Rule:: Rule( const uint32_t i, const string &n ) :
            uuid(i),
            name(n),
            derived(0),
            verbose(false)
            {
            }

            
        }
    }
}
