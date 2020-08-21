
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
        squared( square_of(n) ),
        add_two(n+2)
        {
            assert(n>=5);
        }

        prime:: prime(const utype n) :
        natural(n),
        squared( square_of(n) ),
        add_two(n+2)
        {
            assert(n>=5);
        }



    }

}

