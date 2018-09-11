
#include "y/lang/lexical/unit.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Origin &ruleLabel) :
            Token(), next(0), prev(0), label(ruleLabel)
            {
            }

            Unit:: Unit(const Unit &other):
            Token(other), next(0), prev(0), label(other.label)
            {}

            
        }
    }
}
