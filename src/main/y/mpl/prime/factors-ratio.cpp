
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
            update();
        }

        prime_factors_ratio:: prime_factors_ratio( const word_t n ) :
        num(n),
        den(1)
        {

        }

        prime_factors_ratio:: prime_factors_ratio( const word_t n, const word_t d ) :
        num(n),
        den(d)
        {
            update();
        }


        prime_factors_ratio:: prime_factors_ratio( const prime_factors &n, const prime_factors &d ) :
        num(n),
        den(d)
        {
            update();
        }


        void prime_factors_ratio:: update()
        {
            if( den.is_zero() ) throw exception("prime_factors_ratio division by zero");

            static const MPN     &mp  = MPN::instance();
            static const natural &one = mp._1;

            prime_factors &N = (prime_factors &)num;
            prime_factors &D = (prime_factors &)den;

            if( N.is_zero() )
            {
                //--------------------------------------------------------------
                // zero numerator => force denominator=1
                //--------------------------------------------------------------
                if( ! D.is_one() )
                {
                    D = one;
                }
            }
            else if( N.is_one() )
            {
                //--------------------------------------------------------------
                // do nothing
                //--------------------------------------------------------------
            }
            else
            {
                if( D.is_one() )
                {
                    //----------------------------------------------------------
                    // do nothing!
                    //----------------------------------------------------------
                }
                else
                {
                    //----------------------------------------------------------
                    // N>=2 and D>=2
                    //----------------------------------------------------------
                    prime_factor::db &ndb = N.factors;
                    prime_factor::db &ddb = D.factors;
                    assert( !ndb.search(one) );
                    assert( !ddb.search(one) );

                    list<const natural> removeN;
                    list<const natural> removeD;

                    //----------------------------------------------------------
                    // first pass: scan numerator and record
                    //----------------------------------------------------------
                    const size_t                nn=ndb.size();
                    prime_factor::db::iterator  in=ndb.begin();
                    for(size_t i=nn;i>0;--i,++in)
                    {
                        prime_factor          &n   = **in;
                        prime_factor::pointer *ppD = ddb.search(n.p);
                        if(ppD)
                        {
                            // matching primes
                            prime_factor &d = **ppD;
                            assert(d.p==n.p);

                            if(n.n>d.n)
                            {

                            }
                            else if( n.n<d.n )
                            {

                            }
                            else
                            {
                                // same power
                            }


                        }
                        // else do nothing
                    }

                    //----------------------------------------------------------
                    // second pass: update
                    //----------------------------------------------------------


                }

            }
            



        }


    }

}

