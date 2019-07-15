
#include "y/mpl/prime/factors-ratio.hpp"
#include "y/mpl/mpn.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {

        std::ostream & operator<<( std::ostream &os, const prime_factors_ratio &Q )
        {
            assert( !Q.den.is_zero() );
            if( !Q.den.is_one() )
            {
                os << '(' << Q.num << '/' << '(' << Q.den << ')' << ')';
            }
            else
            {
                os << Q.num;
            }
            return os;
        }

        prime_factors_ratio:: ~prime_factors_ratio() throw()
        {
        }

        
        prime_factors_ratio:: prime_factors_ratio() :
        number_type(),
        num( ),
        den(1)
        {
        }

        prime_factors_ratio:: prime_factors_ratio( const natural &n ) :
        num(n),
        den(1)
        {

        }

        prime_factors_ratio:: prime_factors_ratio( const prime_factors &n ) :
        num(n),
        den(1)
        {

        }

        prime_factors_ratio:: prime_factors_ratio( const natural &n, const natural &d ) :
        num(n),
        den(d)
        {
            if(d.is_zero()) throw libc::exception( EDOM, "prime_factors_ratio division by zero" );
            update();
        }


        prime_factors_ratio:: prime_factors_ratio( const prime_factors &n, const prime_factors &d ) :
        num(n),
        den(d)
        {
            if(d.is_zero()) throw libc::exception( EDOM, "prime_factors_ratio division by zero" );
            update();
        }


        void prime_factors_ratio:: update()
        {
            static const MPN     &mp  = MPN::instance();
            static const natural &one = mp._1;
            assert( !den.is_zero() );
            prime_factors &N = (prime_factors &)num;
            prime_factors &D = (prime_factors &)den;

            if(N.is_zero())
            {
                if( !D.is_one() )
                {
                    D = one;
                }
            }
            else if( N.is_one() )
            {
                // do nothing
            }
            else
            {
                // N>1
                prime_factor::db & ndb = N.factors;
                assert( !ndb.search(one) );

                list<const natural> toRemove;
                



            }


        }


    }

}

