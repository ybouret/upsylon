#include "y/mpl/prime/factors.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace mpl
    {

        size_t prime_factors:: count() const throw()
        {
            return factors.size();
        }

        prime_factors::const_iterator prime_factors::begin() const throw()
        {
            return factors.begin();
        }

        prime_factors::const_iterator prime_factors::end() const throw()
        {
            return factors.end();
        }

        bool prime_factors:: is_zero() const throw()
        {
            return 0 == factors.size();
        }


        bool prime_factors:: is_one()  const throw()
        {
            if( factors.size() == 1)
            {
                const prime_factor &f = **begin();
                return f.p.is_byte(1);
            }
            else
            {
                return 0;
            }
        }

        prime_factors:: ~prime_factors() throw()
        {
        }

        prime_factors:: prime_factors() throw() :
        number_type(),
        factors()
        {

        }

        prime_factors:: prime_factors(const prime_factors &other) :
        number_type(),
        factors( other.factors.size(), as_capacity )
        {
            size_t         nf = other.factors.size();
            const_iterator it = other.factors.begin();
            while( nf-- > 0 )
            {
                const prime_factor::pointer q = new prime_factor(**it);
                if(!factors.insert(q))
                {
                    throw exception("prime_factors( unexpected copy failure )");
                }
                ++it;
            }
        }

        prime_factors & prime_factors:: operator=( const prime_factors &other )
        {

            prime_factors tmp( other );
            factors.swap_table_with(tmp.factors);
            return *this;
        }



        std::ostream & operator<<( std::ostream &os, const prime_factors &pfs )
        {
            const prime_factor::db &db = pfs.factors;
            const size_t            nf = db.size();
            if(nf<=0)
            {
                os << '0';
            }
            else
            {
                prime_factor::db::const_iterator i = db.begin();
                os << *i;
                for(size_t j=2;j<=nf;++j)
                {
                    ++i;
                    os << '*' << *i;
                }
            }
            return os;
        }

        prime_factors:: prime_factors( const natural &x ) :
        factors()
        {
            setup(x);
        }

        prime_factors:: prime_factors( const word_t x) :
        factors()
        {
            const natural _(x);
            setup(_);
        }

        prime_factors:: prime_factors( const char *s ):
        factors()
        {
            const natural _ = natural::parse(s);
            setup(_);
        }

        prime_factors:: prime_factors( const string &s ):
        factors()
        {
            const natural _ = natural::parse(s);
            setup(_);
        }

        natural prime_factors:: get() const
        {
            size_t nf = factors.size();
            if(nf>0)
            {
                const_iterator it   = factors.begin();
                natural        prod = (**it).get();
                for(--nf;nf>0;--nf)
                {
                    ++it;
                    prod *= (**it).get();
                }
                return prod;
            }
            else
            {
                return natural();
            }
        }


    }

}

#include "y/mpl/mpn.hpp"

namespace upsylon
{
    namespace mpl
    {
        void prime_factors:: update()
        {
            static MPN           &mp = MPN::instance();
            static const natural &one = mp._1;

            const size_t nf = factors.size();
            if(nf>0)
            {
                factors.sort_keys( prime_factor::compare_keys );
                prime_factor::pointer *pp = factors.search(one);
                if(pp)
                {
                    if(nf>1)
                    {
                        factors.no(one);
                    }
                    else
                    {
                        (**pp).n = 1;
                    }
                }
            }

        }

        size_t __count( const natural &p, natural &a )
        {
            assert(p<=a);
            //std::cerr << "testing " << a << "/" << p << std::endl;
            size_t ans = 0;
            while( a.is_divisible_by(p) )
            {
                a /= p;
                ++ans;
            }
            return ans;
        }

        void prime_factors:: __add( const natural &p, const size_t n)
        {
            if(n>0)
            {
                const prime_factor::pointer q = new prime_factor(p,n);
                if(!factors.insert(q))
                {
                    throw exception("prime_factors unexpected setup failure");
                }
            }
        }


        void prime_factors:: setup(const natural &x)
        {
            if( x.is_zero() )
            {
                return ;
            }
            else if( x.is_byte(1) )
            {
                __add(x,1);
                return;
            }
            else
            {
                static MPN &mp = MPN::instance();

                // generic case x>1
                natural a = x;

                // start with 2
                natural p = mp._2;
                __add(p,__count(p,a));

                // loop from 3
                p = mp._3;
                while(p<=a)
                {
                    __add(p,__count(p,a));
                    p = mp.nextPrime(p+=mp._2);
                }

                update();
            }
        }


    }

}

namespace upsylon
{
    namespace mpl
    {
        static inline
        void   __ins( prime_factor::db &pf, const prime_factor &f )
        {
            const prime_factor::pointer q = new prime_factor(f);
            if(!pf.insert(q))
            {
                throw exception("prime_factors unexpected copy/insertion failure");
            }
        }
        
        void prime_factors:: mul_by( const prime_factors &other )
        {
            prime_factor::db        prod;
            {
                const prime_factor::db &lhs = factors;
                size_t                  nl  = lhs.size();
                if( nl>0 )
                {
                    const prime_factor::db &rhs = other.factors;
                    size_t                  nr  = rhs.size();
                    if( nr>0 )
                    {

                        // hard copy nl
                        {
                            const_iterator il = lhs.begin();
                            while(nl>0)
                            {
                                __ins(prod,**il);
                                --nl; ++il;
                            }
                        }

                        // multiply
                        {
                            const_iterator ir = rhs.begin();
                            while(nr>0)
                            {
                                const prime_factor    &r   = **ir;
                                prime_factor::pointer *ppL = prod.search(r.p);
                                if(ppL)
                                {
                                    prime_factor &l = **ppL;
                                    l.n += r.n;
                                }
                                else
                                {
                                    __ins(prod,r);
                                }
                                --nr; ++ir;
                            }
                        }

                    }
                }

            }


            prod.swap_table_with(factors);
            update();
        }

        prime_factors prime_factors:: factorial(const natural &n)
        {
            prime_factors p = 1;
            natural i = n;
            while( i > 0 )
            {
                const prime_factors rhs(i);
                p.mul_by(rhs);
                --i;
            }
            return p;
        }

    }
}
