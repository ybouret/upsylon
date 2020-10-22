

#include "y/yap/integer.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // multiplication
        //
        //======================================================================
        template <typename LHS, typename RHS> static inline
        integer mul_proto(const sign_type ls, const LHS &la,
                          const sign_type rs, const RHS &ra)
        {
            const sign_type ps = number::product(ls,rs);
            switch(ps)
            {
                case __zero: return integer();
                default: break;
            }
            const natural n = la*ra;
            return integer(ps,n);
        }

        integer integer:: mul(const integer &lhs, const integer &rhs)
        {
            return mul_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        integer integer::mul(const integer &lhs, const itype rhs)
        {
            const utype ra = iabs_of(rhs);
            return mul_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        integer integer::mul(const itype    lhs, const integer &rhs)
        {
            const utype la = iabs_of(lhs);
            return mul_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }

        integer integer:: mul_by(const natural &rhs) const
        {
            if(rhs<=0)
            {
                return integer();
            }
            else
            {
                switch(s)
                {
                    case __zero:     return integer();
                    case __negative:
                    case __positive: break;
                }
                assert(rhs>0);
                assert(n>0);
                const natural p = rhs*n;
                return integer(s,p);
            }
        }

        integer integer::square_of(const integer &x)
        {
            switch(x.s)
            {
                case __zero: break;
                case __negative:
                case __positive:
                {
                    const natural n2 = natural::square_of(x.n);
                    return integer(__positive,n2);
                }
            }
            return integer();
        }

        integer integer::abs_of(const integer &x)
        {
            switch(x.s)
            {
                case __zero: break;
                case __negative:
                case __positive:
                {
                    return integer(__positive,x.n);
                }
            }
            return integer();
        }

        integer integer::sqrt_of(const integer &x)
        {
            switch(x.s)
            {
                case __zero: break;
                case __negative: throw libc::exception(EDOM,"sqrt(negative integer)");
                case __positive:
                {
                    const natural n = natural::sqrt_of(x.n);
                    return integer(__positive,n);
                }
            }
            return integer();
        }
    }

}

