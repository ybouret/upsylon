
#include "y/mpl/prime/factor.hpp"

namespace upsylon
{
    namespace mpl
    {

        prime_factor:: ~prime_factor() throw()
        {
            n = 0;
        }

        prime_factor:: prime_factor( const natural &prm, const size_t num ) :
        number_type(),
        p(prm),
        n(num)
        {
            assert(n>0);
        }

        prime_factor:: prime_factor( const prime_factor &other ) :
        number_type(),
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

        natural prime_factor:: value() const
        {
            natural ans = p;
            size_t  i   = n;
            for(--i;i>0;--i)
            {
                ans *= p;
            }
            return ans;
        }

        bool operator==( const prime_factor &lhs, const prime_factor &rhs ) throw()
        {
            return (lhs.n==rhs.n) && (lhs.p==rhs.p);
        }

        bool operator!=( const prime_factor &lhs, const prime_factor &rhs ) throw()
        {
            return (lhs.n!=rhs.n) || (lhs.p!=rhs.p);
        }
    }

}

