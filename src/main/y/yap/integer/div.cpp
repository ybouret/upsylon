#include "y/yap/integer.hpp"

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // division
        //
        //======================================================================
        template <typename LHS, typename RHS> static inline
        integer div_proto(const sign_type ls, const LHS &la,
                          const sign_type rs, const RHS &ra)
        {
            const natural n = la/ra;
            return (n>0) ? integer(number::product(ls,rs),n) : integer();
        }

        integer integer:: divide(const integer &lhs, const integer &rhs)
        {
            return div_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        integer integer::divide(const integer &lhs, const itype rhs)
        {
            const utype ra = iabs_of(rhs);
            return div_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        integer integer::divide(const itype    lhs, const integer &rhs)
        {
            const utype la = iabs_of(lhs);
            return div_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }
    }

}

