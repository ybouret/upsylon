#include "y/lang/syntax/internal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Internal:: ~Internal() throw()
            {
            }

            Internal:: Internal(const uint32_t i,
                                const string  &n ) :
            Rule(i,n)
            {
            }

        }
    }
}

