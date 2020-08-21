
#include "y/yap/prime.hpp"
#include "y/exception.hpp"
#include "y/string.hpp"

namespace upsylon
{

    namespace yap
    {
        prime:: ~prime() throw()
        {
        }


        static inline void check_prime( const prime &p )
        {
            if(p<5)
            {
                const string s = p.to_dec();
                throw exception("yap::prime(%s<5)",*s);
            }
        }

#define Y_YAPRIME_CTOR(n) natural(n), next(0), prev(0), squared( square_of(n) ), add_two(n+2)

        prime:: prime(const natural &n) : Y_YAPRIME_CTOR(n)
        {
            check_prime(*this);
        }

        prime:: prime(const utype n) : Y_YAPRIME_CTOR(n)
        {
            check_prime(*this);
        }



    }

}

