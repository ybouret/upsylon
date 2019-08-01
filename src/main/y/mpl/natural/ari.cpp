
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/code/arith.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace mpl
    {
        natural  natural:: mod_inv( const natural &b, const natural &n )
        {
            //std::cerr << "Computing " << b << "^(-1)[" << n << "]" << std::endl;
            natural n0 = n;
            natural b0 = b;
            natural t0;// = 0;
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

        natural natural:: mod_exp( const natural &B, const natural &E, const natural &N )
        {

            if( N.bytes <= 0 )
                throw libc::exception( EDOM, "mpl.mod_exp %% 0");


            natural result = 1;
            if( E.bytes > 0 )
            {
                assert( E.bits() > 0 );
                natural        base  = B;
                const size_t   nbit  = E.bits()-1;
                const uint8_t *ebit  = E.byte;

                for( size_t i=0; i < nbit; ++i )
                {

                    if( ebit[i>>3] & bits_table::value[ i & 7 ] )
                    {
                        //result = ( result * base ) % N;
                        const natural rb = result * base;

                        natural tmp1 = __mod( rb.byte, rb.bytes, N.byte, N.bytes );
                        tmp1.xch( result );
                    }
                    //base = ( base * base ) % N;
                    const natural bsq  = square_of(base);
                    natural       tmp2 = __mod(bsq.byte,bsq.bytes,N.byte,N.bytes);
                    base.xch( tmp2 );
                }

                //-- most significant bit !
                assert( (ebit[nbit>>3] & bits_table::value[ nbit & 7 ]) != 0 );

                //result = ( result * base ) % N;
                const natural rb   = result * base;
                natural       tmp3 = __mod( rb.byte, rb.bytes, N.byte, N.bytes );
                tmp3.xch( result );
            }

            return result;
        }

        bool natural:: are_coprimes(const natural &a, const natural &b)
        {
            if(a.bytes>0&&b.bytes>0)
            {
                const natural g = arithmetic::gcd(a,b);
                return g.is_byte(1);
            }
            else
            {
                return false;
            }

        }

        natural natural:: factorial(const natural &n)
        {
            if(n<=1)
            {
                return natural(1);
            }
            else
            {
                const natural m = n.__dec();
                return n * factorial( m );
            }
        }

        natural natural:: square_root_of(const natural &n)
        {
            if(n.bytes<=sizeof(word_t)/2)
            {
                word_t w = 0;
                memcpy(&w,n.byte,sizeof(word_t));
                w = swap_le(w);
                //std::cerr << "sqrt_fast: n=" << n << ", w=" << std::endl;
                return natural( isqrt(w) );
            }
            else
            {
                //natural x0 = n;
                natural x1 = (n>>1);
                while(true)
                {
                    const natural x0 = x1;
                    x1 = ((x0+n/x0)>>1);
                    if(x1>=x0)
                    {
                        return x0;
                    }
                }
            }
        }

    }

}

