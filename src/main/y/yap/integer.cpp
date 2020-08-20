
#include "y/yap/integer.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/randomized/bits.hpp"
#include <iostream>

namespace upsylon
{

    namespace yap
    {

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
        }

        integer:: integer() : s(__zero), n()
        {
        }

        integer:: ~integer() throw()
        {
            
        }

        integer:: integer(const itype i) : s( sign_of(i) ), n( iabs_of(i) )
        {
        }

        integer::integer(const integer &other) :
        number(),
        s(other.s),
        n(other.n)
        {
            
        }

        integer:: integer(const sign_type S, const natural &N) :
        s(S), n(N)
        {
            if(n<=0)
            {
                if(s!=__zero) throw exception("bad integer setup: zero value, non-zero sign!");
            }
            else
            {
                assert(n>0);
                if(s==__zero) throw exception("bad integer setup: non-zero value, zero sign!");
            }
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

        //======================================================================
        //
        // I/O
        //
        //======================================================================
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

        
        const char integer:: CLASS_NAME[] = "apz";

        const char *integer:: className() const throw() { return CLASS_NAME; }

        size_t integer::serialize(ios::ostream &fp) const
        {
            switch(s)
            {
                case __zero: fp.write( sign2byte(s) ); return 1;
                default:
                    break;
            }
            fp.write( sign2byte(s) );
            return 1 + n.serialize(fp);
        }

        integer integer::read(ios::istream &fp, size_t &delta, const char *which)
        {
            static const char fn[] = "integer::read";
            assert(which);
            delta=0;
            uint8_t b = 0;
            if( !fp.query( (char&)b ) )
            {
                throw exception("%s(missing sign byte for '%s')",fn,which);
            }
            switch(b)
            {
                case __z: delta=1; return integer();
                    
                case __p: {
                    const natural _ = natural::read(fp, delta, which);
                    ++delta;
                    if(_<=0) throw exception("%s(invalid natural for positive '%s')",fn,which);
                    return integer(__positive,_);
                }
                case __n: {
                    const natural _ = natural::read(fp, delta, which);
                    ++delta;
                    if(_<=0) throw exception("%s(invalid natural for negative '%s')",fn,which);
                    return integer(__negative,_);
                }
                default:
                    break;
            }
            throw exception("%s(invalid sign byte for '%s')",fn,which);
        }

        //
        std::ostream & operator<<( std::ostream &os, const integer &z)
        {
            switch(z.s)
            {
                case __zero:     os << '0'; break;
                case __negative: os << '-' << z.n; break;
                case __positive: os << z.n; break;
            }
            return os;
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

#define Y_APZ_SIGNS(A,B) ( (A<<8) | B )
        static unsigned signs2flag(const unsigned l, const unsigned r) throw()
        {
            return Y_APZ_SIGNS(l,r);
        }

        template <typename LHS, typename RHS> static inline
        int cmp_proto(const sign_type ls, const LHS &la,
                      const sign_type rs, const RHS &ra) throw()
        {
            const unsigned flag = signs2flag( integer::sign2byte(ls), integer::sign2byte(rs) );
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
