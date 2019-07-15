
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/code/arith.hpp"

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

#if 0
        bool natural:: is_prime( const natural &n )
        {
            static const MPN &_ = MPN::instance();

            if(n<=_._1)
                return false; //!< for 0 and 1
            else if(n<=_._3)
                return true;  //!< for 2 and 3
            else if( n.is_divisible_by_byte(2) || n.is_divisible_by_byte(3) ) //! for 2,4,6,8,9...
                return false;
            else
            {
                assert(n>=_._5);
                natural i = _._5;
                for(;;)
                {
                    const natural isq = natural::square_of(i);
                    if(isq>n) break;
                    if( n.is_divisible_by(i) ) return false;
                    const natural j=i+_._2;
                    if( n.is_divisible_by(j) )  return false;
                    i += _._6;
                }
                return true;
            }
        }

        natural natural:: next_prime(const natural &n)
        {
            static const MPN &_ = MPN::instance();
            if(n<=_._2)
            {
                return _._2;
            }
            else
            {
                natural p = n|_._1;
                while( !is_prime(p) ) p+= _._2;
                return p;
            }
        }
#endif

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

    }

}

