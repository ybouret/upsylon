#include "y/mpl/rational.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        void rational:: check()
        {
            if(den.is_zero())
            {
                throw libc::exception(EDOM,"mpl.rational: zero denominator");
            }
            __simplify();
        }

        void rational:: __simplify()
        {
            assert(den.is_positive());
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
                    natural      &n = (natural &)(z.n);
                    natural      a=n;
                    natural      b=d;
                    if(b>a)
                    {
                        a.xch(b);
                    }
                    // b<=a
                    while( b.is_positive() )
                    {
                        natural m = natural::__mod(a,b);
                        a.xch(b);
                        b.xch(m);
                    }
                    // a is the gcd
                    n/=a;
                    d/=a;
                }
            }
        }
    }
}
