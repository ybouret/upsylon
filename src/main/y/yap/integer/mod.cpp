#include "y/yap/integer.hpp"

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // modulo
        //
        //======================================================================
        template <typename LHS, typename RHS> static inline
        integer mod_proto(const LHS &lhs, const RHS &rhs)
        {
            const integer q = lhs/rhs;
            const integer qr = q*rhs;
            return lhs-qr;
        }

        integer integer:: modulo(const integer &lhs, const integer &rhs)
        {
            return mod_proto(lhs,rhs);
        }

        integer integer::modulo(const integer &lhs, const itype rhs)
        {
            return mod_proto(lhs,rhs);
        }

        integer integer::modulo(const itype    lhs, const integer &rhs)
        {
            return mod_proto(lhs,rhs);
        }

    }

}

