
#include "y/yap/prime/miller.hpp"
#include "y/yap/library.hpp"

#include "y/yap/prime/sprp.hpp"

namespace upsylon
{

    namespace yap
    {

        bool Miller:: is_prime(const natural &n)
        {
            static const library &apl = library::instance();
            static const natural &ini = apl._2;

            if(n<=apl._3)
            {
                // may be 0,1,2,3
                return (n>apl._1);
            }
            else
            {
                if( n.is_even()|| n.is_divisible_by(apl._3) )
                {
                    return false;
                }
                else
                {
                    assert(n>=5);
                    assert(n.is_odd());
                    const sprp    chk(n);
                    const natural end = sprp::end(n);
                    for(natural a=ini;a<end;++a)
                    {
                        if( !chk.base(a) ) return false;
                    }
                    return true;
                }

            }

        }

        bool Miller:: is_prime(const number::utype u)
        {
            const natural _(u);
            return is_prime(_);
        }

    }

}
