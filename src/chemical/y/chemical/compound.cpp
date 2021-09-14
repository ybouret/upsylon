
#include "y/chemical/compound.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Compound:: ~Compound() throw()
        {
        }

        Compound:: Compound(const Actor &a, const Tier t) throw() :
        Object(),
        authority<const Actor>(a),
        tier(t)
        {

        }




    }

}
