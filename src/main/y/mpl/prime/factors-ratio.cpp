
#include "y/mpl/prime/factors-ratio.hpp"
#include "y/mpl/mpn.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {

        void prime_factors_ratio:: xch( prime_factors_ratio &Q ) throw()
        {
            ((prime_factors &)num).xch( (prime_factors &)(Q.num));
            ((prime_factors &)den).xch( (prime_factors &)(Q.den));
        }

        prime_factors_ratio:: prime_factors_ratio( const prime_factors_ratio &Q ) :
        num( Q.num ),
        den( Q.den )
        {
        }

        prime_factors_ratio &  prime_factors_ratio::  operator=( const prime_factors_ratio &Q )
        {
            prime_factors_ratio tmp(Q);
            xch(tmp);
            return *this;
        }


        std::ostream & operator<<( std::ostream &os, const prime_factors_ratio &Q )
        {
            assert( !Q.den.is_zero() );
            if( !Q.den.is_one() )
            {
                const bool paren = ( Q.den.count() > 1);
                os << '(' << Q.num << '/';
                if(paren) os << '(';
                os << Q.den;
                if(paren) os << ')';
                os << ')';
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
            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            if( den.is_zero() )
            {
                throw exception("prime_factors_ratio division by zero");
            }

            //------------------------------------------------------------------
            // local constants
            //------------------------------------------------------------------
            static const MPN     &mp  = MPN::instance();
            static const natural &one = mp._1;

            //------------------------------------------------------------------
            // get N and D
            //------------------------------------------------------------------
            prime_factors &N = (prime_factors &)num;
            prime_factors &D = (prime_factors &)den;

            if( N.is_zero() )
            {
                //--------------------------------------------------------------
                /// zero numerator: force denominator to 1
                //--------------------------------------------------------------
                if(!D.is_one())
                {
                    prime_factors tmp = one;
                    D.xch(tmp);
                }
            }
            else if( N.is_one() )
            {
                //--------------------------------------------------------------
                // do nothing for numerator is 1
                //--------------------------------------------------------------
            }
            else
            {
                //--------------------------------------------------------------
                // N >= 2 at this point
                //--------------------------------------------------------------
                if( D.is_one() )
                {
                    //----------------------------------------------------------
                    // do nothing since D=1
                    //----------------------------------------------------------
                }
                else
                {
                    //----------------------------------------------------------
                    // N>=2, Q>=2
                    //----------------------------------------------------------
                    prime_factors new_num = 1;
                    prime_factors new_den = 1;

                    size_t nn = N.count();
                    size_t nd = D.count();
                    prime_factors::const_iterator in = N.begin();
                    prime_factors::const_iterator id = D.begin();

                    // check common part
                    while( (nd>0) && (nn>0) )
                    {
                        const prime_factor &fn = **in;
                        const prime_factor &fd = **id;

                        if( fn.p == fd.p )
                        {
                            const natural &p = fn.p;
                            if( fn.n < fd.n )
                            {
                                // only on den
                                new_den.__add(p,fd.n-fn.n);
                            }
                            else if(fd.n<fn.n)
                            {
                                // only on num
                                new_num.__add(p,fn.n-fd.n);
                            }
                            else
                            {
                                assert(fn.n==fd.n); // do nothing
                            }
                        }
                        else
                        {
                            new_num.__add(fn.p,fn.n);
                            new_den.__add(fd.p,fd.n);
                        }

                        --nd; ++id;
                        --nn; ++in;
                    }

                    // then fill up denominator
                    while(nd>0)
                    {
                        const prime_factor &fd = **id;
                        new_den.__add(fd.p,fd.n);
                        --nd;++id;
                    }

                    // and fill up numerator
                    while(nn>0)
                    {
                        const prime_factor &fn = **in;
                        new_num.__add(fn.p,fn.n);
                        --nn;++in;
                    }

                    new_num.update();
                    new_den.update();

                    N.xch(new_num);
                    D.xch(new_den);

                }

            }

        }


        void prime_factors_ratio:: mul_by( const prime_factors_ratio &x )
        {
            prime_factors new_num = num; new_num.mul_by(x.num);
            prime_factors new_den = den; new_den.mul_by(x.den);
            prime_factors_ratio Q(new_num,new_den);
            xch(Q);
        }

        void prime_factors_ratio:: div_by( const prime_factors_ratio &x )
        {
            prime_factors       new_num = num;      new_num.mul_by(x.den);
            prime_factors       new_den = den;      new_den.mul_by(x.num);
            prime_factors_ratio Q(new_num,new_den); xch(Q);
        }

#define Y_PFQ_MULOPS_BY(TYPE) \
void prime_factors_ratio:: mul_by( const TYPE x )\
{\
prime_factors       new_num = x;    new_num.mul_by(num);\
prime_factors_ratio Q(new_num,den); xch(Q);\
}\
void prime_factors_ratio :: div_by( const TYPE x )\
{\
prime_factors       new_den = x;    new_den.mul_by(den);\
prime_factors_ratio Q(num,new_den); xch(Q);\
}
        Y_PFQ_MULOPS_BY(prime_factors&)
        Y_PFQ_MULOPS_BY(natural&)
        Y_PFQ_MULOPS_BY(word_t)


    }

}

