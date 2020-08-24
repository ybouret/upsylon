
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
            const prime_factors::factors_type fac = F.factors;
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
            factors_type tmp(other.factors);
            aliasing::_(factors).swap_with(tmp);
            return *this;
        }

        prime_factors:: prime_factors(const natural &n) :
        factors()
        {
            static library &apl = library::instance();
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
                        //std::cerr << "|_testing " << num << "/" << den << std::endl;
                        assert(den<=num);
                        while(true)
                        {
                            natural::split(q,r,num,den);
                            //std::cerr << " |_q=" << q << ", r=" << r << std::endl;
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

    }

}

