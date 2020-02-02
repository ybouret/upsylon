#include "y/lang/syntax/rule/internal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Internal:: ~Internal() throw()
            {
            }

            Internal:: Internal(const uint32_t        i,
                                const string         &n,
                                const std::type_info &j) :
            Rule(i,n,j)
            {
            }

        }
    }
}

