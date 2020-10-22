
#include "y/yap/integer.hpp"
#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/randomized/bits.hpp"
#include "y/os/error.hpp"
#include <iostream>
#include <cerrno>

namespace upsylon
{

    namespace yap
    {

        unsigned integer::signs2flag(const unsigned l, const unsigned r) throw()
        {
            return Y_APZ_SIGNS(l,r);
        }

        void integer:: cast_overflow(const char *which)
        {
            if(!which) which = "???";
            throw exception("integer cast overflow for '%s'",which);
        }
        
#define Y_APZ_CHECK(HOST) assert( ((HOST).s == __zero) || ((HOST).n>0) )
        //======================================================================
        //
        // C++/setup
        //
        //======================================================================

        void integer:: update() throw()
        {
            if(n<=0)
            {
                aliasing::_(s) = __zero;
            }
            Y_APZ_CHECK(*this);
        }

        integer:: integer() : s(__zero), n()
        {
            Y_APZ_CHECK(*this);
        }

        integer:: ~integer() throw()
        {
            Y_APZ_CHECK(*this);
        }

        integer:: integer(const itype i) : s( sign_of(i) ), n( iabs_of(i) )
        {
            Y_APZ_CHECK(*this);
        }

        integer::integer(const integer &other) :
        number(),
        s(other.s),
        n(other.n)
        {
            Y_APZ_CHECK(*this);
        }

        static inline
        void check_manual_setup(const integer &i)
        {
            if(i.n<=0)
            {
                if(i.s!=__zero) throw exception("bad integer setup: zero value, non-zero sign!");
            }
            else
            {
                assert(i.n>0);
                if(i.s==__zero) throw exception("bad integer setup: non-zero value, zero sign!");
            }
        }

        integer:: integer(const sign_type S, const natural &N) :
        s(S), n(N)
        {
            check_manual_setup(*this);
            Y_APZ_CHECK(*this);
        }

        integer:: integer(const natural &u) : s( (0==u) ? __zero : __positive ), n(u)
        {
        }
        

        integer:: integer(const sign_type S, const utype U) :
        s(S), n(U)
        {
            check_manual_setup(*this);
            Y_APZ_CHECK(*this);
        }




        integer:: integer(const sign_type S, randomized::bits &ran, const size_t nbits) :
        s(S), n(ran,nbits)
        {
            update();
        }

        integer:: integer(randomized::bits &ran, const size_t nbits) :
        s(ran.choice()?__positive:__negative), n(ran,nbits)
        {
            update();
        }



        void integer:: xch(integer &other) throw()
        {
            _cswap(s,other.s);
            aliasing::_(n).xch( aliasing::_(other.n) );
        }

        integer & integer:: operator=(const integer &other)
        {
            integer tmp(other);
            xch(tmp);
            return *this;
        }


        integer & integer:: operator=(const itype i)
        {
            integer tmp(i);
            xch(tmp);
            return *this;
        }

        uint8_t integer:: sign2byte(const sign_type s) throw()
        {
            switch(s)
            {
                case __negative: return uint8_t(__n);
                case __zero:     return uint8_t(__z);
                case __positive: return uint8_t(__p);
            }
            return 0xff; // never get here
        }


        

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

#include "y/string.hpp"

namespace upsylon
{

    namespace yap
    {
        string integer:: to_dec() const
        {
            if(s==__negative)
            {
                return '-' + n.to_dec();
            }
            else
            {
                return n.to_dec();
            }
        }


    }

}
