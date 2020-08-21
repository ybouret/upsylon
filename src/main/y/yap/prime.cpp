
#include "y/yap/prime.hpp"

namespace upsylon
{

    namespace yap
    {
        prime:: ~prime() throw()
        {
        }

        prime:: prime(const natural &n) :
        natural(n),
        q( square_of(n) )
        {
            assert(n>=2);
        }

        prime:: prime(const utype n) :
        natural(n),
        q( square_of(n) )
        {
            assert(n>=2);
        }



    }

}

