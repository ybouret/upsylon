
#include "y/yap/prime/factor.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"
#include <iostream>

namespace upsylon {

    namespace yap {

        void prime_factor:: setup() throw()
        {
            if(1==p) aliasing::_(n)=1;
        }


#define Y_APPF_CTOR()\
next(0), \
p(prm),  \
n(xpn)

        prime_factor :: prime_factor(const natural &prm, const size_t xpn) :
        Y_APPF_CTOR()
        {
            assert(n>0);
            setup();
        }

        prime_factor :: prime_factor(const utype prm, const size_t xpn) :
        Y_APPF_CTOR()
        {
            assert(n>0);
            setup();
        }

        prime_factor:: ~prime_factor() throw()
        {
            aliasing::_(n) = 0;
        }

        prime_factor:: prime_factor(const prime_factor &f) throw() :
        number(),
        next(0),
        p(f.p),
        n(f.n)
        {

        }

        natural prime_factor:: value() const
        {
            const natural &x   = p;
            natural        ans = x;
            size_t         cnt = n;
            while(--cnt>0)
            {
                ans *= x;
            }
            return ans;
        }

        prime_factor * prime_factor:: make_one() throw()
        {
            aliasing::_(p).make(1);
            aliasing::_(n)=1;
            return this;
        }

        

        bool operator==(const prime_factor &lhs, const prime_factor &rhs) throw()
        {
            return (lhs.p==rhs.p) && (lhs.n==rhs.n);
        }


        bool operator!=(const prime_factor &lhs, const prime_factor &rhs) throw()
        {
            return (lhs.p!=rhs.p) || (lhs.n!=rhs.n);
        }

        std::ostream & operator<<(std::ostream &os, const prime_factor &f)
        {
            if(f.n<=1)
            {
                assert(1==f.n);
                os << f.p;
            }
            else
            {
                os << '(' << f.p << '^' << f.n << ')';
            }
            return os;
        }

        prime_factor & prime_factor:: operator=(const prime_factor &) throw()
        {

            return *this;
        }

        const char prime_factor:: CLASS_NAME[] = "yapf";

        const char * prime_factor:: className() const throw() { return CLASS_NAME; }

        size_t prime_factor:: serialize(ios::ostream &fp) const
        {
            const size_t hdr = fp.write_upack(n);
            return hdr + p.serialize(fp);
        }

        prime_factor  prime_factor:: read(ios::istream &fp, size_t &delta, const char *which)
        {
            static const char fn[] = "prime_factor::read: ";
            assert(which);
            size_t xpn_delta = 0;
            size_t xpn       = 0;
            if( !fp.query_upack(xpn,xpn_delta) ) throw exception("%sno exponent for '%s'",fn,which);
            if(xpn<=0) throw exception("%scorrupted exponent for '%s'",fn,which);
            size_t        prm_delta = 0;
            const natural prm       = natural::read(fp,prm_delta, which);
            if(prm==0)  throw exception("%scorrupted possible prime for '%s'",fn,which);
            delta = xpn_delta + prm_delta;
            return prime_factor(prm,xpn);
        }

    }

}
