
#include "y/mpl/prime/factor.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"
#include "y/string.hpp"

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

        const char prime_factor:: CLASS_NAME[] = "prmf";

        const char * prime_factor:: className() const throw() { return CLASS_NAME; }

        size_t prime_factor:: serialize(ios::ostream &fp) const
        {
            const size_t bytes_for_p = p.serialize(fp);
            return bytes_for_p + fp.write_upack(n);
        }

        prime_factor prime_factor:: read( ios::istream &fp, size_t *shift, const string &which)
        {
            static const char fn[] = "read";
            size_t np = 0, nn=0;
            string reason = which + " prime";
            const natural prm = natural::read(fp,&np,reason);
            if(prm<=0)                   throw exception("%s::%s(invalid prime for '%s')",fn,CLASS_NAME,*which);
           
            size_t        num = 0;
            if(!fp.query_upack(num,&nn)) throw exception("%s::%s(missing exponent for '%s')",fn,CLASS_NAME,*which);
            if(num<=0)                   throw exception("%s::%s(invalid exponent for '%s')",fn,CLASS_NAME,*which);
            
            ios::gist::assign(shift,np+nn);
            return prime_factor(prm,num);
        }

    }

}

