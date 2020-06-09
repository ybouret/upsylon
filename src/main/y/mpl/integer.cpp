
#include "y/mpl/integer.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"

#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        sign_type sign_ops:: product(const sign_type a, const sign_type b) throw()
        {
            if(a==__zero||b==__zero)
            {
                return __zero;
            }
            else
            {
                return (a!=b) ? __negative : __positive;
            }
        }

        sign_type sign_ops:: of(const integer_t i) throw()
        {
            return (i<0) ? __negative : ( (0<i)  ? __positive : __zero );
        }

        sign_type sign_ops:: of(const natural &u)  throw()
        {
            return (u.is_zero()) ? __zero : __positive;
        }

        word_t  sign_ops:: i2w(const integer_t i) throw()
        {
            return (i<0) ? word_t(-i) : word_t(i);
        }

        sign_type sign_ops:: neg(const sign_type s) throw()
        {
            switch(s)
            {
                case __negative: return __positive;
                case __positive: return __negative;
                default:         break;
            }
            return __zero;

            //return (s == __negative) ? __positive : ( s == __positive ? __negative : __zero );
        }
    }

}

#include "y/type/aliasing.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace mpl
    {

        integer::  integer() : s(__zero), n() {}

        integer:: ~integer() throw() { (sign_type&)s = __zero; }

        integer::  integer(const integer &z) : object(), number_type(), s(z.s), n(z.n) {}

        integer::integer(const integer_t i) : object(), number_type(), s( sign_ops::of(i) ), n( sign_ops::i2w(i) ) {}

        integer:: integer(const natural &u) : object(), number_type(), s( sign_ops::of(u) ), n(u) {}

        integer::integer(const sign_type _s, const natural &u ) : object(), number_type(), s( u.is_zero() ? __zero : _s), n(u) {}

        integer:: integer(const size_t nbit, randomized::bits &gen) : object(), number_type(), s( gen.choice() ? __negative : __positive ), n(nbit,gen)
        {
            update();
        }


        void  integer::update() throw()
        {
            if(n.is_zero()) (sign_type &)s = __zero;
        }

        std::ostream & operator<<( std::ostream &os, const integer &z )
        {
            if(__negative==z.s) os << '-';
            return (os << z.n);
        }




        integer_t  integer::lsi() const throw()
        {
            switch(s)
            {
                case __negative: return -integer_t(n.lsw());
                case __zero:     return 0;
                case __positive: return  integer_t(n.lsw());
            }
            fatal_error("corrupted code@mpl.integer.lsi");
            return 0;

        }

        void  integer:: xch( integer &other ) throw()
        {
            _cswap(s,other.s);
            aliasing::_(n).xch( aliasing::_(other.n) );
        }

        //! assign
        integer & integer::operator=( const integer   &z) { integer tmp(z); xch(tmp); return *this; }
        //! assign
        integer & integer::operator=( const integer_t  i) { integer tmp(i); xch(tmp); return *this; }
        //! assign
        integer & integer::operator=( const natural   &u) { integer tmp(u); xch(tmp); return *this; }

        //! signed comparison
        int integer::compare_blocks(const sign_type ls,
                                    const uint8_t  *l,
                                    const size_t    nl,
                                    const sign_type rs,
                                    const uint8_t  *r,
                                    const size_t    nr) throw()
        {
            switch(ls)
            {
                case __negative: switch(rs)
                {
                    case __negative: return -natural::compare_blocks(l,nl,r,nr);  //ls<0,rs<0
                    case __zero:    /* FALLTHRU */ // ls<0, rs=0
                    case __positive: return -1;    // ls<0, rs>0
                } /* FALLTHRU */

                case __zero: switch(rs)
                {
                    case __negative: return  1; // ls=0,rs<0
                    case __zero:     return  0; // ls=0,rs=0
                    case __positive: return -1; // ls=0,rs>0
                } /* FALLTHRU */

                case __positive: switch(rs)
                {
                    case __negative:               // ls<0,rs>0
                    case __zero:      return 1;    // ls=0,rs>0
                    case __positive:  return natural::compare_blocks(l,nl,r,nr); //ls>0,rs>0
                }
            }

            fatal_error("corrupted code@mpl.integer.compare_blocks");
            return 0;
        }

        const uint8_t *integer:: prepare( integer_t &i, sign_type &si, size_t &ni ) throw()
        {
            assert(__zero==si);
            assert(0==ni);
            if(i<0)
            {
                si = __negative;
                i  = -i;
                return natural::prepare((word_t&)i,ni);
            }
            else if(i>0)
            {
                si = __positive;
                return natural::prepare((word_t&)i,ni);
            }
            else
            {
                return (const uint8_t*)i;
            }
        }

        integer integer::operator+() { return *this; }

        integer integer::__inc() const
        {
            const uint8_t b = 1;
            return __add(this->s,this->n.byte,this->n.bytes,__positive,&b,1);
        }

        integer & integer::operator++()    { integer tmp = __inc(); xch(tmp); return *this; }

        integer   integer::operator++(int) { integer tmp = __inc(); xch(tmp); return tmp;   }
        

        integer integer::operator-() const { return integer(sign_ops::neg(s),n); }

        integer integer::__dec() const
        {
            const uint8_t b = 1;
            return __add(this->s,this->n.byte,this->n.bytes,__negative,&b,1);
        }

        integer & integer::operator--() { integer tmp = __dec(); xch(tmp); return *this; }

        integer   integer::operator--(int) { integer tmp = __dec(); xch(tmp); return tmp; }

        integer integer:: __add(const sign_type ls,
                                const uint8_t  *l,
                                const size_t    nl,
                                const sign_type rs,
                                const uint8_t  *r,
                                const size_t    nr)
        {
            switch(ls)
            {
                case __negative: switch(rs)
                {
                    case __negative: { const natural S = natural::__add(l,nl,r,nr); assert(!S.is_zero()); return integer(__negative,S); } // ls<0,rs<0
                    case __zero:     { const natural L(l,nl);                       assert(!L.is_zero()); return integer(__negative,L); } // ls<0,rs=0
                    case __positive: // ls<0,rhs>0
                        switch( natural::compare_blocks(l,nl,r,nr) )
                    {
                        case -1: // |l|<|r|
                        { const natural D = natural::__sub(r,nr,l,nl); assert(!D.is_zero()); return integer(D); }
                        case  1: // |l|>|r|
                        { const natural D = natural::__sub(l,nl,r,nr); assert(!D.is_zero()); return integer(__negative,D); }
                        default: // |l| = |r|
                            return integer();
                    }
                } /* FALLTHRU */

                case __zero: switch(rs)
                {
                    case __negative: { natural R(r,nr); assert(!R.is_zero()); return integer(__negative,R);  } // ls=0,rs<0
                    case __zero:      return integer();                                                        // ls=0,rs=0
                    case __positive: { natural R(r,nr); assert(!R.is_zero()); return integer(R);             } // ls=0,rs>0
                } /* FALLTHRU */

                case __positive:switch(rs)
                {
                    case __negative:
                        switch( natural::compare_blocks(l,nl,r,nr) )
                    {
                        case -1: // |l| < |r|
                        { const natural D = natural::__sub(r,nr,l,nl); assert(!D.is_zero()); return integer(__negative,D); }
                        case 1: // |l| > |r|
                        { const natural D = natural::__sub(l,nl,r,nr); assert(!D.is_zero()); return integer(D); }
                        default: // |l| = |r|
                            return integer();
                    }
                    case __zero: ;   { natural L(l,nl);                       assert(!L.is_zero()); return integer(L); } // ls>0,rs=0
                    case __positive: { natural S = natural::__add(l,nl,r,nr); assert(!S.is_zero()); return integer(S); } // ls>0,rs>0
                }
            }
            fatal_error("corrupted code@mpl.integer.add");
            return integer();
        }

        integer integer:: __sub(const sign_type ls,
                                const uint8_t  *l,
                                const size_t    nl,
                                const sign_type rs,
                                const uint8_t  *r,
                                const size_t    nr)
        {
            return __add(ls,l,nl,sign_ops::neg(rs),r,nr);
        }

        integer integer:: __mul(const sign_type ls,
                                const uint8_t  *l,
                                const size_t    nl,
                                const sign_type rs,
                                const uint8_t  *r,
                                const size_t    nr)
        {
            switch(ls)
            {
                case __negative: switch(rs)
                {
                    case __negative: { const natural p = natural::__mul(l,nl,r,nr); assert(!p.is_zero()); return integer(p);             }
                    case __zero:     {                                                                    return integer();              }
                    case __positive: { const natural p = natural::__mul(l,nl,r,nr); assert(!p.is_zero()); return integer(__negative,p);  }
                } /* FALLTHRU */

                case __zero:
                    return integer();

                case __positive: switch(rs)
                {
                    case __negative: { const natural p = natural::__mul(l,nl,r,nr); assert(!p.is_zero()); return integer(__negative,p);  }
                    case __zero:     {                                                                    return integer();              }
                    case __positive: { const natural p = natural::__mul(l,nl,r,nr); assert(!p.is_zero()); return integer(p);             }
                }
            }
            fatal_error("corrupted code@mpl.interger.mul");
            return integer();            }

        integer integer::__div(const sign_type ls,
                               const uint8_t  *l,
                               const size_t    nl,
                               const sign_type rs,
                               const uint8_t  *r,
                               const size_t    nr)
        {
            switch(ls)
            {
                case __negative: switch(rs)
                {
                    case __negative: { const natural p = natural::__div(l,nl,r,nr); return integer(p);             }
                    case __zero:     goto DIV_BY_ZERO;
                    case __positive: { const natural p = natural::__div(l,nl,r,nr);  return integer(__negative,p); }
                } /* FALLTHRU */

                case __zero:
                    return integer();

                case __positive: switch(rs)
                {
                    case __negative: { const natural p = natural::__div(l,nl,r,nr); return integer(__negative,p);  }
                    case __zero:    goto DIV_BY_ZERO;
                    case __positive: { const natural p = natural::__div(l,nl,r,nr); return integer(p);             }
                }
            }
        DIV_BY_ZERO:
            throw libc::exception(EDOM,"mpl.integer(division by zero)");
        }

        integer integer:: square_of( const integer &z )
        {
            const natural n2 = natural::square_of(z.n);
            return integer(n2);
        }

        bool integer:: is_divisible_by(const natural &d) const
        {
            return n.is_divisible_by(d);
        }



    }

}


namespace upsylon
{
    namespace mkl
    {
        mpz sqrt_of(const mpz &z)
        {
            if( mpl::__negative == z.s )
            {
                throw libc::exception(EDOM,"mpl.integer(square root of negative value)");
            }
            const  mpn sz = sqrt_of(z.n);
            return mpz(z.s,sz);
        }
    }
}

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace mpl
    {
        const char integer:: CLASS_NAME[] = "mpz";

        const char * integer:: className() const throw()
        {
            return CLASS_NAME;
        }

        size_t integer:: serialize( ios::ostream &fp ) const
        {
            uint8_t      s_code      = 0x00;
            switch(s)
            {
                case __negative: s_code=0xff; break;
                case __zero:     s_code=0x00; break;
                case __positive: s_code=0x01; break;
            }
            const size_t bytes_for_s = fp.write_nbo(s_code);
            return  bytes_for_s + n.serialize(fp);
        }

        integer integer:: read(ios::istream &fp, size_t &shift, const char *which)
        {
            static const char fn[] = "integer::read";
            assert(which);
            sign_type         _s = __zero;
            {
                uint8_t u = 0;
                if(!fp.query_nbo(u,shift)) throw exception("%s(missing sign for '%s')",fn,which);
                assert(1==shift);
                switch(u)
                {
                    case 0xff: _s = __negative; break;
                    case 0x00: _s = __zero;     break;
                    case 0x01: _s = __positive; break;
                    default:
                        throw exception("%s(invalid sign=%u for '%s')",fn,unsigned(u),which);
                }
            }
            size_t       shift2 = 0;
            const mpn   _n      = natural::read(fp,shift2,which);
            shift += shift2;
            return integer(_s,_n);
        }
    }

}

#include "y/string.hpp"

namespace upsylon {

        namespace mpl {

            string  integer:: to_decimal() const
            {
                string s;
                if(__negative==s)
                {
                    s << '-';
                }
                s << n.to_decimal();
                return s;
            }
        }
}


namespace upsylon {

    namespace mpl {

        template <typename ITYPE,typename UTYPE> static inline
        bool __to( ITYPE &z, const integer &i ) throw()
        {

            static const UTYPE umax = static_cast<UTYPE>(  limit_of<ITYPE>::maximum    );
            static const UTYPE umin = static_cast<UTYPE>(-(limit_of<ITYPE>::minimum+1) )+1;
            //std::cerr << "umax=" << (unsigned long long)(umax) << ", umin=" << (unsigned long long)(umin) << std::endl;
            UTYPE u = 0;
            switch(i.s)
            {
                case __zero:     z=0; return true;
                case __positive:
                    if(!i.n.to(u) || u>umax)
                    {
                        return false;
                    }
                    else
                    {
                        z = static_cast<ITYPE>(u);
                        return true;
                    }
                case __negative:
                {
                    if(!i.n.to(u) || u>umin )
                    {
                        return false;
                    }
                    else
                    {
                        switch(u)
                        {
                            case umin: z = limit_of<ITYPE>::minimum; break;
                            default  : z = -static_cast<ITYPE>(u);   break;
                        }
                        return true;
                    }
                }
            }
            return false; //!< never get here

        }

        template <>
        bool integer:: to<int8_t>(int8_t &z) const throw()
        {
            return __to<int8_t,uint8_t>(z,*this);
        }

        template <>
        bool integer:: to<int16_t>(int16_t &z) const throw()
        {
            return __to<int16_t,uint16_t>(z,*this);

        }

        template <>
        bool integer:: to<int32_t>(int32_t &z) const throw()
        {
            return __to<int32_t,uint32_t>(z,*this);
        }

        template <>
        bool integer:: to<int64_t>(int64_t &z) const throw()
        {
            return __to<int64_t,uint64_t>(z,*this);
        }

        void integer:: throw_cast_overflow(const char *when) const
        {
            throw libc::exception( ERANGE, "mpz.cast overflow %s", (when ? when : "!") );
        }
    }
}
