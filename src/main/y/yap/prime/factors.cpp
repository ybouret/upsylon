
#include "y/yap/prime/factors.hpp"
#include "y/yap/prime/iterator.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace yap {

        prime_factors:: prime_factors() throw() :
        factors()
        {
        }

        prime_factors:: ~prime_factors() throw()
        {
        }

        std::ostream & operator<<(std::ostream &os, const prime_factors &F)
        {
            const pf_list &fac = F.factors;
            switch (fac.size)
            {
                case 0: os << '0'; break;
                case 1: os << *fac.head; break;
                default:
                {
                    os<< '(';
                    for(const prime_factor *cur = fac.head, *nxt=cur->next;cur;)
                    {
                        os << (*cur);
                        if(nxt)
                        {
                            os << '*';
                            cur=nxt;
                            nxt=nxt->next;
                        }
                        else
                        {
                            break;
                        }
                    }
                    os << ')';
                }
            }

            return os;
        }

        bool   prime_factors:: is_zero() const throw()
        {
            return factors.size<=0;
        }

        bool prime_factors:: is_one() const throw()
        {
            return 1==factors.size && (1==factors.head->p);
        }

        void prime_factors::xch(prime_factors &other) throw()
        {
            aliasing::_(factors).swap_with( aliasing::_(other.factors) );
        }

        natural prime_factors:: value() const
        {
            if(factors.size<=0)
            {
                return natural();
            }
            else
            {
                const prime_factor *pf  = factors.head;
                natural             ans = pf->value();
                for(pf=pf->next;pf;pf=pf->next)
                {
                    const natural tmp = pf->value();
                    ans *= tmp;
                }
                return ans;
            }
        }

        prime_factors:: prime_factors(const prime_factors &other) :
        number(),
        factors(other.factors)
        {

        }

        prime_factors & prime_factors:: operator=( const prime_factors &other )
        {
            pf_list tmp(other.factors);
            aliasing::_(factors).swap_with(tmp);
            return *this;
        }

        void prime_factors:: make(const natural &n)
        {
            static library       &apl  = library::instance();
            static const natural &one  = apl._1;
            static const natural &zero = apl._0;
            if(n==zero)
            {
                // do nothing
            }
            else
            {
                if(n==one)
                {
                    aliasing::_(factors).push_back( new prime_factor(1,1) );
                }
                else
                {
                    natural        num = n,q,r;
                    prime_iterator it(apl);
                    while(num!=one)
                    {
                        size_t         xpn = 0;
                        const natural &den = *it;
                        assert(den<=num);
                        while(true)
                        {
                            natural::split(q,r,num,den);
                            if(r==zero)
                            {
                                ++xpn;
                                num.xch(q);
                            }
                            else
                            {
                                break;
                            }
                        }
                        if(xpn>0)
                        {
                            aliasing::_(factors).push_back( new prime_factor(den,xpn) );
                        }
                        ++it;
                    }
                }
            }
        }

        prime_factors:: prime_factors(const utype u) :
        factors()
        {
            const natural n(u);
            make(n);
        }
        
        prime_factors:: prime_factors(const natural &n) :
        factors()
        {
            make(n);
        }

        const char prime_factors:: CLASS_NAME[] = "yapF";

        const char * prime_factors:: className() const throw() { return CLASS_NAME; }

        size_t       prime_factors:: serialize(ios::ostream &fp) const
        {
            size_t ans = fp.write_upack(factors.size);
            for(const prime_factor *f=factors.head;f;f=f->next)
            {
                ans += f->serialize(fp);
            }
            return ans;
        }


        prime_factors prime_factors:: mul(const prime_factors &lhs, const prime_factors &rhs)
        {
            static library       &apl  = library::instance();
            static const natural &one  = apl._1;
            const pf_list        &l    = lhs.factors; const size_t nl = l.size;
            const pf_list        &r    = rhs.factors; const size_t nr = r.size;

            if(nl<=0||nr<=0)
            {
                return prime_factors();
            }
            else
            {
                assert(nl>0);
                assert(nr>0);
                prime_factors       p;
                pf_list            &P = aliasing::_(p.factors);
                const prime_factor *L = l.head;
                const prime_factor *R = r.head;

                while(L&&R)
                {
                    switch(natural::scmp(L->p,R->p))
                    {
                        case __negative: P.push_back( new prime_factor(*L) ); L=L->next; break;
                        case __zero:     P.push_back( new prime_factor(L->p,L->n+R->n) ); L=L->next; R=R->next; break;
                        case __positive: P.push_back( new prime_factor(*R) ); R=R->next; break;
                    }
                }

                assert(NULL==L||NULL==R);
                while(L)
                {
                    P.push_back( new prime_factor(*L) ); L=L->next;
                }
                while(R)
                {
                    P.push_back( new prime_factor(*R) ); R=R->next;
                }

                if(P.size>1&&one==P.head->p)
                {
                    assert(1==P.head->n);
                    delete P.pop_front();
                }


                return p;
            }
        }

        prime_factors prime_factors:: mul(const prime_factors &lhs, const natural &rhs)
        {
            const prime_factors _(rhs);
            return mul(lhs,_);
        }

        prime_factors prime_factors:: mul(const prime_factors &lhs, const utype rhs)
        {
            const prime_factors _(rhs);
            return mul(lhs,_);
        }


    }

}

