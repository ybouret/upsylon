
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

        bool natural:: coprime(const natural &lhs, const natural &rhs)
        {
            if(lhs.bytes>0&&rhs.bytes>0)
            {
                assert(lhs>0); assert(rhs>0);
                const natural g = gcd(lhs,rhs);
                return g.bytes == 1 && g.word[0] == 1;
            }
            else
            {
                return false;
            }
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

        natural  natural:: mod_inv( const natural &b, const natural &n )
        {
            //std::cerr << "Computing " << b << "^(-1)[" << n << "]" << std::endl;
            natural n0 = n;
            natural b0 = b;
            natural t0 = 0;
            natural t  = 1;
            natural q  = n0/b0;
            natural r  = n0 - q*b0;

            while( r>0 )
            {
                const natural lhs = t0;
                const natural rhs = q*t;

                t0 = t;

                if( compare(lhs,rhs) >= 0 )
                {
                    assert(lhs>=rhs);
                    t = ( (lhs-rhs) % n );
                    assert(t<n);
                }
                else
                {
                    assert(rhs>lhs);
                    t  = n - ( (rhs-lhs)%n );
                    assert(t<=n);
                }

                n0 = b0;
                b0 = r;
                q  = n0/b0;
                r  = n0 - q * b0;
            }

            // normally an error if b0 != 1
            //std::cerr << b << "*" << t << "[" << n << "]=" << (b*t)%n << std::endl;
            return t;
        }

#if 0
        natural natural:: mod_exp( const natural &B, const natural &E, const natural &N )
        {

            if( N.bytes <= 0 )
                throw libc::exception( EDOM, "natural::mod_exp %% 0");


            natural result = 1;
            if( E.bytes > 0 )
            {
                assert( E.bits() > 0 );
                natural        base  = B;
                const size_t   nbit  = E.bits()-1;
                const uint8_t *ebit  = E.byte;

                for( size_t i=0; i < nbit; ++i )
                {

                    if( ebit[i>>3] & bits_table::value[ (i&0x7) ] )
                    {
                        //result = ( result * base ) % N;
                        const natural rb   = result * base;
                        natural       tmp1 = __mod( rb.byte, rb.bytes, N.byte, N.bytes );
                        tmp1.xch( result );
                    }

                    //base = ( base * base ) % N;
                    const natural bsq  = square_of(base);
                    natural       tmp2 = __mod(bsq.byte,bsq.bytes,N.byte,N.bytes);
                    base.xch( tmp2 );
                }

                //-- most significant bit !
                assert( (ebit[nbit>>3] & bits_table::value[ (nbit&7) ]) != 0 );

                //result = ( result * base ) % N;
                {
                    const natural rb   = result * base;
                    natural       tmp3 = __mod( rb.byte, rb.bytes, N.byte, N.bytes );
                    tmp3.xch( result );
                }
            }

            return result;
        }
#endif

    }
}
