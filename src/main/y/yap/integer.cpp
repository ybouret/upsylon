
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

        // io

        uint8_t integer:: sign2byte(const sign_type s) throw()
        {
            switch(s)
            {
                case __zero:     return 0x00;
                case __positive: return 0x01;
                case __negative: return 0x02;
            }
            return 0xff; // never get here
        }

        sign_type integer:: byte2sign(const uint8_t u)
        {
            switch(u)
            {
                case 0x00: return __zero;
                case 0x01: return __positive;
                case 0x02: return __negative;
            }
            throw exception("integer(invalid byte sign 0x%02x)",u);
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
                case 0x00: delta=1; return integer();
                case 0x01: {
                    const natural _ = natural::read(fp, delta, which);
                    ++delta;
                    if(_<=0) throw exception("%s(invalid natural for positive '%s')",fn,which);
                    return integer(__positive,_);
                }
                case 0x02: {
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

    }

}
