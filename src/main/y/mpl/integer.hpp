//! \file
#ifndef Y_MP_INTEGER_INCLUDED
#define Y_MP_INTEGER_INCLUDED 1

#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! named sign
        enum sign_type
        {
            __negative, //!< -1
            __zero,     //!<  0
            __positive  //!<  1
        };

        //! product of signs
        inline sign_type sign_product(const sign_type a, const sign_type b) throw()
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

        //! sign for integral type
        inline sign_type sign_for(const integer_t i) throw()
        {
            return (i<0) ? __negative : ( (0<i)  ? __positive : __zero );
        }

        //! absolute value
        inline word_t word_for(const integer_t i) throw()
        {
            return (i<0) ? word_t(-i) : word_t(i);
        }

        //! negative sign
        inline sign_type sign_neg( const sign_type s ) throw()
        {
            switch(s)
            {
                case __negative: return __positive;
                case __zero:     return __zero;
                case __positive: return __negative;
            }
        }

        //! integer class
        class integer : public object
        {
        public:
            const sign_type s; //!< it sign
            const natural   n; //!< it natural value

            //! zero constructor
            inline integer() : s(__zero), n() {}

            //! destructor
            inline virtual ~integer() throw() {}

            //! copy constructor
            inline integer(const integer &z) : s(z.s), n(z.n) {}

            //!  constructor from integral
            inline integer(const integer_t &i) : s( sign_for(i) ), n( word_for(i) ) {}

            //! constructor from natural
            inline integer(const natural &u) : s( u.is_zero() ? __zero : __positive ), n(u) {}

            //! status update
            inline void update() throw()
            {
                if(n.is_zero()) (sign_type &)s = __zero;
            }

            //! output
            inline friend std::ostream & operator<<( std::ostream &os, const integer &z )
            {
                if(__negative==z.s) os << '-';
                return (os << z.n);
            }

            //! least significant int
            inline integer_t lsi() const throw()
            {
                switch(s)
                {
                    case __negative: return -integer_t(n.lsw());
                    case __zero:     return 0;
                    case __positive: return  integer_t(n.lsw());
                }
            }
            //! no throw exchange
            inline void xch( integer &other ) throw()
            {
                cswap(s,other.s);
                ((natural &)n).xch((natural &)(other.n));
            }

            //! assign
            inline integer & operator=( const integer   &z) { integer tmp(z); xch(tmp); return *this; }
            //! assign
            inline integer & operator=( const integer_t  i) { integer tmp(i); xch(tmp); return *this; }
            //! assign
            inline integer & operator=( const natural   &u) { integer tmp(u); xch(tmp); return *this; }

            //! signed comparison
            static int compare_blocks(const sign_type ls,
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
                        case __zero:                   // ls<0, rs=0
                        case __positive: return -1;    // ls<0, rs>0
                    }

                    case __zero: switch(rs)
                    {
                        case __negative: return  1; // ls=0,rs<0
                        case __zero:     return  0; // ls=0,rs=0
                        case __positive: return -1; // ls=0,rs>0
                    }

                    case __positive: switch(rs)
                    {
                        case __negative:               // ls<0,rs>0
                        case __zero:      return 1;    // ls=0,rs>0
                        case __positive:  return natural::compare_blocks(l,nl,r,nr); //ls>0,rs>0
                    }
                }
            }

            static inline const uint8_t *prepare( integer_t &i, sign_type &si, size_t &ni ) throw()
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

#define Y_MPZ_PREPARE() sign_type si = __zero; size_t ni=0; const uint8_t *bi = prepare(i,si,ni)
#define Y_MPZ_ZARGS(V)  V.s,V.n.byte,V.n.bytes
#define Y_MPZ_IARGS()   si,bi,ni
#define Y_MPZ_IMPL_NOTHROW(RET,BODY,CALL) \
inline RET BODY(const integer &lhs, const integer  &rhs) throw() { return CALL(Y_MPZ_ZARGS(lhs), Y_MPZ_ZARGS(rhs) ); }\
inline RET BODY(const integer &lhs, integer_t       i  ) throw() { Y_MPZ_PREPARE(); return CALL(Y_MPZ_ZARGS(lhs),Y_MPZ_IARGS()); }\
inline RET BODY(integer_t      i,   const integer  &rhs) throw() { Y_MPZ_PREPARE(); return CALL(Y_MPZ_IARGS(),Y_MPZ_ZARGS(rhs)); }

            Y_MPZ_IMPL_NOTHROW(static int,compare,compare_blocks)

            //! comparison operator declarations
#define Y_MPZ_CMP(OP) \
inline friend bool operator OP ( const integer  &lhs, const integer   &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const integer  &lhs, const integer_t  rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const integer_t lhs, const integer   &rhs ) throw() { return compare(lhs,rhs) OP 0; }

            Y_MPZ_CMP(==)
            Y_MPZ_CMP(!=)
            Y_MPZ_CMP(<=)
            Y_MPZ_CMP(<)
            Y_MPZ_CMP(>=)
            Y_MPZ_CMP(>)


        };

    }

    typedef mpl::integer mpz;
}


#endif

