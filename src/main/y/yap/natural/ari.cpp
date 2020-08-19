
#include "y/yap/library.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace yap
    {

        natural natural:: sqrt_of(const natural &n)
        {
            if( n<=1 )
            {
                return n;
            }
            else
            {
                //natural x1 = (n>>1);
                natural x1 = n;
                (void) x1.shr(1);
                while(true)
                {
                    const natural x0 = x1;
                    //x1 = ((x0+n/x0)>>1);
                    x1 = n/x0;
                    x1 += x0;
                    (void) x1.shr(1);
                    if(x1>=x0)
                    {
                        return x0;
                    }
                }
            }
        }

        natural natural:: sqrt_of(const utype u)
        {
            const natural _(u);
            return sqrt_of(_);
        }

        natural natural:: factorial(size_t n)
        {
            natural ans = 1;
            while(n>1)
            {
                ans *= n;
                --n;
            }
            return ans;
        }

        natural natural:: comb(const size_t n, const size_t k)
        {
            if(k>n)
            {
                throw libc::exception(EDOM,"natural::comb(k=%lu>n=%lu)",(unsigned long)k, (unsigned long)n);
            }
            natural ans = factorial(n);
            {
                const natural fk = factorial(k); assert(ans.is_divisible_by(fk));
                ans /= fk;
            }
            {
                const natural fnmk = factorial(n-k); assert(ans.is_divisible_by(fnmk));
                ans /= fnmk;
            }
            return ans;
        }

        natural natural:: gcd(const natural &x, const natural &y)
        {
            assert(x>0);
            assert(y>0);
            bool swp = false;
            switch(scmp(x,y))
            {
                case __negative: assert(x<y);  swp = true; break;
                case __zero:     assert(x==y); return x;
                case __positive: assert(x>y);  break;
            }
            natural a = x;
            natural b = y;
            if(swp) a.xch(b);
            assert(a>b);
            while(b.bytes>0)
            {
                natural t = b;
                b = a % b;
                a.xch(t);
            }
            return a;
        }

        void natural:: simplify(natural &num, natural &den)
        {
            static const library &apl = library::instance();
            if(den.bytes<=0)
            {
                throw libc::exception(EDOM,"simplify by zero");
            }
            else
            {
                assert(den>0);
                if(num.bytes<=0)
                {
                    assert(0==num);
                    den = apl._1;
                }
                else
                {
                    assert(num>0);
                    const natural g  = gcd(num,den);
                    natural       nn = num/g;
                    natural       dd = den/g;
                    nn.xch(num);
                    dd.xch(den);
                }
            }
        }


    }
}
