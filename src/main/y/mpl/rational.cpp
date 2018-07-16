#include "y/mpl/rational.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/code/arith.hpp"

namespace upsylon
{
    namespace mpl
    {
        void rational:: check()
        {
            if(den.is_zero())
            {
                throw libc::exception(EDOM,"mpl.rational: nul denominator");
            }
            __simplify();
        }

        void rational:: __simplify()
        {
            assert(den>0);
            natural &d = (natural &) den;
            integer &z = (integer &) num;
            if(z.s==__zero)
            {
                // numerator is null => denominator to 1
                assert(true==z.n.is_zero());
                if(!d.is_byte(1))
                {
                    d.set_byte(1);
                }
            }
            else
            {
                // numerator is not zero, obviously not for denominator
                assert( z.s   != __zero        );
                assert( false == z.n.is_zero() );

                if(!d.is_byte(1))
                {
                    natural      &n = (natural &)(num.n);
                    const natural g = arithmetic::gcd_positive(n,d);
                    n /= g;
                    d /= g;
                }
            }
        }
    }
}
