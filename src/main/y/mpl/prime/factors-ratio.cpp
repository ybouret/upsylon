
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
        number_type(),
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

                    // construct biased factors
                    prime_factors new_num = one;
                    prime_factors new_den = one;

                    {
                        for(prime_factors::const_iterator in=N.begin();in!=N.end();++in)
                        {
                            const prime_factor &fn = **in;
                            if(fn.p!=one) new_num.__add(fn.p,fn.n);
                        }


                        for(prime_factors::const_iterator id=D.begin();id!=D.end();++id)
                        {
                            const prime_factor &fd = **id;
                            if(fd.p!=one) new_den.__add(fd.p,fd.n);
                        }
                    }


                    // first pass: detect common factors and adjust
                    list<mpn> bad_all;
                    list<mpn> bad_den;
                    list<mpn> bad_num;

                    prime_factors::const_iterator in=new_num.begin();
                    assert( one == (**in).p );
                    for(++in;in!=new_num.end();++in)
                    {
                        const prime_factor  &fn = **in;
                        const natural       &p  = fn.p;
                        const prime_factor::pointer *ppfd = new_den.factors.search(p);
                        if(ppfd)
                        {
                            const prime_factor &fd = **ppfd;
                            size_t             &nx = (size_t &)(fn.n);
                            size_t             &dx = (size_t &)(fd.n);
                            if(nx<dx)
                            {
                                bad_num.push_back(p);
                                dx -= nx;
                            }
                            else if(dx<nx)
                            {
                                bad_den.push_back(p);
                                nx -= dx;
                            }
                            else
                            {
                                assert(nx==dx);
                                bad_all.push_back(p);
                            }
                        }
                    }

                    // second pass: remove factors
                    while( bad_num.size() )
                    {
                        new_num.factors.no( bad_num.back() );
                        bad_num.pop_back();
                    }
                    bad_num.release();

                    while( bad_den.size() )
                    {
                        new_den.factors.no( bad_den.back() );
                        bad_den.pop_back();
                    }
                    bad_den.release();

                    while( bad_all.size() )
                    {
                        const natural &p = bad_all.back();
                        new_den.factors.no( p );
                        new_num.factors.no( p );
                        bad_all.pop_back();
                    }
                    bad_all.release();


                    // then update all
                    new_num.update();
                    new_den.update();

                    // and swap
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


        prime_factors prime_factors:: perm(const natural &n, const natural &k)
        {
            if(k>n)
            {
                return prime_factors();
            }
            else
            {
                const natural             nmk = n-k;
                const prime_factors       num = factorial(n);
                const prime_factors       den = factorial(n-k);
                const prime_factors_ratio Q(num,den);
                if( !Q.den.is_one() ) throw libc::exception( EDOM, "prime_factor::perm failure");
                return Q.num;
            }
        }

        prime_factors prime_factors:: comb(const natural &n, const natural &k)
        {
            if(k>n)
            {
                return prime_factors();
            }
            else
            {
                const natural             nmk = n-k;
                const prime_factors       num = factorial(n);
                const prime_factors       den = factorial(n-k);
                prime_factors_ratio       Q(num,den);
                const prime_factors       kf  = factorial(k);
                Q.div_by(kf);
                if( !Q.den.is_one() ) throw libc::exception( EDOM, "prime_factor::comb failure");
                return Q.num;
            }
        }


        rational prime_factors_ratio:: value() const
        {
            const natural n = num.value();
            const natural d = den.value();
            return rational(n,d);
        }

        const char   prime_factors_ratio:: CLASS_NAME[] = "mpQ";
        const char * prime_factors_ratio:: className() const throw() { return CLASS_NAME; }

        size_t prime_factors_ratio:: serialize(ios::ostream &fp) const
        {
            const size_t bytes_for_num = num.serialize(fp);
            const size_t bytes_for_den = den.serialize(fp);
            return bytes_for_den+bytes_for_num;
        }



        bool operator==( const prime_factors_ratio &lhs, const prime_factors_ratio &rhs) throw()
        {
            return (lhs.num==rhs.num) && (lhs.den==rhs.den);
        }

        bool operator!=( const prime_factors_ratio &lhs, const prime_factors_ratio &rhs) throw()
        {
            return (lhs.num!=rhs.num) || (lhs.den!=rhs.den);
        }

    }

}

#include "y/ios/ostream.hpp"
namespace upsylon
{
    namespace mpl
    {
        prime_factors_ratio prime_factors_ratio::  read(ios::istream &fp, size_t *shift)
        {
            size_t nn = 0, nd=0;
            const prime_factors _num = mpF::read(fp, &nn);
            const prime_factors _den = mpF::read(fp, &nd);
            ios::gist::assign(shift,nn+nd);
            return prime_factors_ratio(_num,_den);
        }
    }
}
