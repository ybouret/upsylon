#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/ios/serialized.hpp"

namespace upsylon
{
    namespace Lang
    {

#if 0
        bool Pattern:: AreEqual(const Pattern &lhs, const Pattern &rhs) throw()
        {
            const uint32_t lid = lhs.uuid;
            const uint32_t rid = rhs.uuid;
            if(lid!=rid)
            {
                return false;
            }
            else if( Single::UUID == lid )
            {
                return static_cast<Single *>(lhs.priv)->code == static_cast<Single *>(rhs.priv)->code;
            }
            else
            {
                // generic case
                return ios::serialized::are_same_binary(lhs,rhs);
            }


        }
#endif
        
    }

}

