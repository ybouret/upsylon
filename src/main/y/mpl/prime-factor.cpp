
#include "y/mpl/prime-factor.hpp"

namespace upsylon
{
    namespace mpl
    {

        prime_factor:: ~prime_factor() throw()
        {
            n = 0;
        }

        prime_factor:: prime_factor( const natural &prm, const size_t num ) :
        p(prm),
        n(num)
        {
            assert(n>0);
        }

        prime_factor:: prime_factor( const prime_factor &other ) :
        p( other.p ),
        n( other.n )
        {
        }


        std::ostream & operator<<( std::ostream &os, const prime_factor &pf )
        {
            const size_t n = pf.n;
            if(n>1)
            {
                os << '(' << pf.p << '^' << n << ')';
            }
            else
            {
                os << pf.p;
            }
            return os;
        }

        const natural & prime_factor:: key() const throw()
        {
            return p;
        }

        int prime_factor:: compare_keys( const natural &lhs, const natural &rhs ) throw()
        {
            return natural::compare(lhs,rhs);
        }

        natural prime_factor:: get() const
        {
            natural ans = p;
            size_t  i   = n;
            for(--i;i>0;--i)
            {
                ans *= p;
            }
            return ans;
        }


    }

}

#include "y/exception.hpp"

namespace upsylon
{
    namespace mpl
    {
        prime_factors:: ~prime_factors() throw()
        {
        }

        prime_factors:: prime_factors() throw() : factors()
        {

        }

        prime_factors:: prime_factors(const prime_factors &other) :
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

        void prime_factors:: update()
        {
            factors.sort_keys( prime_factor::compare_keys );
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
                assert( !(p.is_byte(1) && n!=1) );
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
