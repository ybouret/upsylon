
#include "y/yap/integer.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace yap
    {
        //======================================================================
        //
        // addition
        //
        //======================================================================
        template <typename LHS, typename RHS> static inline
        integer add_proto(const sign_type ls, const LHS &la,
                          const sign_type rs, const RHS &ra)
        {
            const unsigned flag = integer::signs2flag( integer::sign2byte(ls), integer::sign2byte(rs) );
            switch(flag)
            {
                    //----------------------------------------------------------
                    // lhs<0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__n,integer::__n):
                {
                    const natural s = la+ra;
                    return integer(__negative,s);
                }
                case Y_APZ_SIGNS(integer::__n,integer::__z):
                {
                    return integer(ls,la);
                }
                case Y_APZ_SIGNS(integer::__n,integer::__p):
                {
                    switch( natural::scmp(la,ra) )
                    {
                        case __negative: assert(la<ra); { const natural n=ra-la; return integer(__positive,n); }
                        case __zero:     assert(la==ra);{                        return integer();             }
                        case __positive: assert(ra<la); { const natural n=la-ra; return integer(__negative,n); }
                    }
                } break;

                    //----------------------------------------------------------
                    // lhs==0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__z,integer::__n): return  integer(rs,ra);
                case Y_APZ_SIGNS(integer::__z,integer::__z): return  integer();
                case Y_APZ_SIGNS(integer::__z,integer::__p): return  integer(rs,ra);

                    //----------------------------------------------------------
                    // lhs>0
                    //----------------------------------------------------------
                case Y_APZ_SIGNS(integer::__p,integer::__n):
                {
                    switch( natural::scmp(la,ra) )
                    {
                        case __negative: assert(la<ra); { const natural n=ra-la; return integer(__negative,n); }
                        case __zero:     assert(la==ra); return integer();
                        case __positive: assert(ra<la); { const natural n=la-ra; return integer(__positive,n); }
                    }
                } break;

                case Y_APZ_SIGNS(integer::__p,integer::__z):
                {
                    return  integer(ls,la);
                }
                case Y_APZ_SIGNS(integer::__p,integer::__p):
                {
                    const natural s = la+ra;
                    return integer(__positive,s);
                }

                default:
                    break;
            }
            throw exception("yap::integer::add_proto(corrupted signs)");
        }

        integer integer:: add(const integer &lhs, const integer &rhs)
        {
            return add_proto(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        integer integer::add(const integer &lhs, const itype rhs)
        {
            const utype ra = iabs_of(rhs);
            return add_proto(lhs.s,lhs.n, sign_of(rhs),ra);
        }

        integer integer::add(const itype    lhs, const integer &rhs)
        {
            const utype la = iabs_of(lhs);
            return add_proto(sign_of(lhs),la,rhs.s,rhs.n);
        }


        integer integer:: operator+() const
        {
            return integer(*this);
        }

        integer & integer:: operator++()
        {
            static const utype one = 1;
            integer tmp = add_proto(s,n,__positive,one);
            xch(tmp);
            return *this;
        }

        integer  integer:: operator++(int)
        {
            static const utype one = 1;
            const integer      ans(*this);
            {
                integer tmp = add_proto(s,n,__positive,one);
                xch(tmp);
            }
            return ans;
        }

        //======================================================================
        //
        // subtraction
        //
        //======================================================================
        integer integer:: sub(const integer &lhs, const integer &rhs)
        {
            return add_proto(lhs.s,lhs.n,opposite(rhs.s),rhs.n);
        }

        integer integer::sub(const integer &lhs, const itype rhs)
        {
            const utype ra = iabs_of(rhs);
            return add_proto(lhs.s,lhs.n, opposite(sign_of(rhs)),ra);
        }

        integer integer::sub(const itype    lhs, const integer &rhs)
        {
            const utype la = iabs_of(lhs);
            return add_proto(sign_of(lhs),la,opposite(rhs.s),rhs.n);
        }


        integer integer:: operator-() const
        {
            return integer(opposite(s),n);
        }

        integer & integer:: operator--()
        {
            static const utype one = 1;
            integer tmp = add_proto(s,n,__negative,one);
            xch(tmp);
            return *this;
        }

        integer  integer:: operator--(int)
        {
            static const utype one = 1;
            const integer      ans(*this);
            {
                integer tmp = add_proto(s,n,__negative,one);
                xch(tmp);
            }
            return ans;
        }


    }

}

