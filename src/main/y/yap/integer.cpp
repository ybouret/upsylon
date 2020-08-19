
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

        
        const char integer:: CLASS_NAME[] = "yapz";

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
        // eq/neq
        //
        //======================================================================
        
        bool integer::eq(const integer &lhs, const integer &rhs) throw()
        {
            return (lhs.s==rhs.s) && (lhs.n==rhs.n);
        }
        
        bool integer::eq(const integer &lhs, const itype rhs) throw()
        {
            return (lhs.s==sign_of(rhs)) && (lhs.n==iabs_of(rhs));
        }
        
        bool integer::eq(const itype    lhs, const integer &rhs) throw()
        {
            return (sign_of(lhs)==rhs.s) && (iabs_of(lhs)==rhs.n);
        }
        
        bool integer::neq(const integer &lhs, const integer &rhs) throw()
        {
            return !((lhs.s==rhs.s) && (lhs.n==rhs.n) );
        }
        
        bool integer::neq(const integer &lhs, const itype rhs) throw()
        {
            return !((lhs.s==sign_of(rhs)) && (lhs.n==iabs_of(rhs)));
        }
        
        bool integer::neq(const itype    lhs, const integer &rhs) throw()
        {
            return !((sign_of(lhs)==rhs.s) && (iabs_of(lhs)==rhs.n));
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
        
        int integer::cmp(const integer &lhs, const integer &rhs) throw()
        {
            const unsigned flag = signs2flag( sign2byte(lhs.s), sign2byte(rhs.s) );
            switch(flag)
            {
                case Y_APZ_SIGNS(__z,__z): return 0;
                case Y_APZ_SIGNS(__z,__p): return -1;
                
            }
            
            switch(lhs.s)
            {
                case __negative:
                    switch(rhs.s)
                    {
                        case __zero:     return  -1; // (lhs<0) < (rhs=0)
                        case __negative: return   natural::cmp(rhs.n,lhs.n); // (lhs<0) ,  (rhs<0)
                        case __positive: return  -1; // (lhs<0) <  (rhs>0)
                    }
                    break;
                    
                case __zero:
                    switch(rhs.s)
                    {
                        case __zero:     return  0; // (lhs=0) == (rhs=0)
                        case __negative: return  1; // (lhs=0) >  (rhs<0)
                        case __positive: return -1; // (lhs=0) <  (rhs>0)
                    }
                    break;
                    
                case __positive:
                    switch(rhs.s)
                    {
                        case __zero:     return   1; // (lhs>0) >  (rhs=0)
                        case __negative: return   1; // (lhs>0) >  (rhs<0)
                        case __positive: return  natural::cmp(lhs.n,rhs.n); // (lhs>0) ,  (rhs>0)
                    }
                    break;
            }
            
            
        }
        
    }

}
