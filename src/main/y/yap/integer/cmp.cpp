
#include "y/yap/integer.hpp"
#include "y/os/error.hpp"

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // eq
        //
        //======================================================================

        template <typename LHS, typename RHS> static inline
        bool eq_proto(const sign_type ls, const LHS &la,
                      const sign_type rs, const RHS &ra) throw()
        {
            return (ls==rs) && (la==ra);
        }

        bool integer::eq(const integer &lhs, const integer &rhs) throw()
        {
            return eq_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        bool integer::eq(const integer &lhs, const itype rhs) throw()
        {
            const utype ra = iabs_of(rhs);
            return eq_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        bool integer::eq(const itype    lhs, const integer &rhs) throw()
        {
            const utype la = iabs_of(lhs);
            return eq_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }

        //======================================================================
        //
        // neq
        //
        //======================================================================

        template <typename LHS, typename RHS> static inline
        bool neq_proto(const sign_type ls, const LHS &la,
                       const sign_type rs, const RHS &ra) throw()
        {
            return (ls!=rs) || (la!=ra);
        }

        bool integer::neq(const integer &lhs, const integer &rhs) throw()
        {
            return neq_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        bool integer::neq(const integer &lhs, const itype rhs) throw()
        {
            const utype ra = iabs_of(rhs);
            return neq_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        bool integer::neq(const itype    lhs, const integer &rhs) throw()
        {
            const utype la = iabs_of(lhs);
            return neq_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }

        //======================================================================
        //
        // comparison
        //
        //======================================================================



        template <typename LHS, typename RHS> static inline
        int cmp_proto(const sign_type ls, const LHS &la,
                      const sign_type rs, const RHS &ra) throw()
        {
            const unsigned flag = integer::signs2flag( integer::sign2byte(ls), integer::sign2byte(rs) );
            switch(flag)
            {
                    //----------------------------------------------------------
                    // lhs<0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__n,integer::__n): return natural::cmp(ra,la);
                case Y_APZ_SIGNS(integer::__n,integer::__z): return -1;
                case Y_APZ_SIGNS(integer::__n,integer::__p): return -1;

                    //----------------------------------------------------------
                    // lhs==0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__z,integer::__n): return  1;
                case Y_APZ_SIGNS(integer::__z,integer::__z): return  0;
                case Y_APZ_SIGNS(integer::__z,integer::__p): return -1;

                    //----------------------------------------------------------
                    // lhs>0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__p,integer::__n): return  1;
                case Y_APZ_SIGNS(integer::__p,integer::__z): return  1;
                case Y_APZ_SIGNS(integer::__p,integer::__p): return natural::cmp(la,ra);

                default:
                    break;
            }
            fatal_error("yap::integer::cmp_proto(corrupted signs)");
            return 0;
        }


        int integer::cmp(const integer &lhs, const integer &rhs) throw()
        {
            return cmp_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        int integer::cmp(const integer &lhs, const itype rhs) throw()
        {
            const utype ra = iabs_of(rhs);
            return cmp_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        int integer::cmp(const itype lhs, const integer &rhs) throw()
        {
            const utype la = iabs_of(lhs);
            return cmp_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }
    }

}

