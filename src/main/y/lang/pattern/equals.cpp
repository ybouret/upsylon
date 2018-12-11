#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"

namespace upsylon
{
    namespace Lang
    {

        bool Pattern:: AreEqual(const Pattern &lhs, const Pattern &rhs) throw()
        {
            const uint32_t lid = lhs.uuid;
            const uint32_t rid = rhs.uuid;
            if(lid!=rid)
            {
                return false;
            }

            switch(lid)
            {
                case Single::UUID: return static_cast<Single *>(lhs.priv)->code == static_cast<Single *>(rhs.priv)->code;

                default:
                    break;
            };

            return false;
        }

    }

}

