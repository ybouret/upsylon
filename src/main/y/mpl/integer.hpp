//! \file
#ifndef Y_MP_INTEGER_INCLUDED
#define Y_MP_INTEGER_INCLUDED 1

#include "y/mpl/natural.hpp"
#include "y/os/error.hpp"

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

        //! sign for natural
        inline sign_type sign_for(const natural &u) throw()
        {
            return (u.is_zero()) ? __zero : __positive;
        }

        //! absolute value
        inline word_t word_for(const integer_t i) throw()
        {
            return (i<0) ? word_t(-i) : word_t(i);
        }

        //! negative sign
        inline sign_type sign_neg( const sign_type s ) throw()
        {
            return (s == __negative) ? __positive : ( s == __positive ? __negative : __zero );
        }



        //! integer class
        class integer : public number_type
        {
        public:
            const sign_type s; //!< it sign
            const natural   n; //!< it natural value

            //! zero constructor
            inline integer() : s(__zero), n() {}

            //! destructor
            inline virtual ~integer() throw() {}

            //! copy constructor
            inline integer(const integer &z) : object(), number_type(), s(z.s), n(z.n) {}

            //!  constructor from integral
            inline integer(const integer_t &i) : object(), number_type(), s( sign_for(i) ), n( word_for(i) ) {}

            //! constructor from natural
            inline integer(const natural &u) : object(), number_type(), s( sign_for(u) ), n(u) {}
            
            //! create from a natural and a given sign
            inline integer(const sign_type _s, const natural &u ) : object(), number_type(), s( u.is_zero() ? __zero : _s), n(u) {}

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
				fatal_error("corrupted code@mpl.integer.lsi");
				return 0;

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

                fatal_error("corrupted code@mpl.interger.compare_blocks");
                return 0;
            }

            //! binary preparation
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
            //! raw data from integert_t
#define Y_MPZ_PREPARE() sign_type si = __zero; size_t ni=0; const uint8_t *bi = prepare(i,si,ni)
            //! present arguments from integer
#define Y_MPZ_ZARGS(V)  V.s,V.n.byte,V.n.bytes
            //! present arguments from integer_t
#define Y_MPZ_IARGS()   si,bi,ni
            //! present arguments from natura;
#define Y_MPZ_UARGS()   sign_for(u),u.byte,u.bytes
            //! multiple implementation
#define Y_MPZ_IMPL_NOTHROW(RET,BODY,CALL) \
inline RET BODY(const integer &lhs, const integer  &rhs) throw() { return CALL(Y_MPZ_ZARGS(lhs), Y_MPZ_ZARGS(rhs) ); }\
inline RET BODY(const integer &lhs, integer_t       i  ) throw() { Y_MPZ_PREPARE(); return CALL(Y_MPZ_ZARGS(lhs),Y_MPZ_IARGS()); }\
inline RET BODY(integer_t      i,   const integer  &rhs) throw() { Y_MPZ_PREPARE(); return CALL(Y_MPZ_IARGS(),Y_MPZ_ZARGS(rhs)); }\
inline RET BODY(const natural &u,   const integer  &rhs) throw() { return CALL(Y_MPZ_UARGS(),Y_MPZ_ZARGS(rhs)); }\
inline RET BODY(const integer &lhs, const natural  &u  ) throw() { return CALL(Y_MPZ_ZARGS(lhs),Y_MPZ_UARGS()); }

            Y_MPZ_IMPL_NOTHROW(static int,compare,compare_blocks)

            //! comparison operator declarations
#define Y_MPZ_CMP(OP) \
inline friend bool operator OP ( const integer  &lhs, const integer   &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const integer  &lhs, const integer_t  rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const integer_t lhs, const integer   &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const natural  &lhs, const integer   &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const integer  &lhs, const natural   &rhs ) throw() { return compare(lhs,rhs) OP 0; }
            Y_MPZ_CMP(==)
            Y_MPZ_CMP(!=)
            Y_MPZ_CMP(<=)
            Y_MPZ_CMP(<)
            Y_MPZ_CMP(>=)
            Y_MPZ_CMP(>)

            //! define wrappers
#define Y_MPZ_DEFINE(RET,BODY) \
static inline RET BODY(const integer &lhs, const integer  &rhs)   {                       return BODY(Y_MPZ_ZARGS(lhs), Y_MPZ_ZARGS(rhs) ); }\
static inline RET BODY(const integer &lhs, integer_t       i  )   { Y_MPZ_PREPARE();      return BODY(Y_MPZ_ZARGS(lhs),Y_MPZ_IARGS());      }\
static inline RET BODY(integer_t      i,   const integer  &rhs)   { Y_MPZ_PREPARE();      return BODY(Y_MPZ_IARGS(),Y_MPZ_ZARGS(rhs));      }\
static inline RET BODY(const natural  &u,  const integer &rhs )   { const integer lhs(u); return BODY(Y_MPZ_ZARGS(lhs), Y_MPZ_ZARGS(rhs) ); } \
static inline RET BODY(const integer &lhs, const natural &u   )   { const integer rhs(u); return BODY(Y_MPZ_ZARGS(lhs), Y_MPZ_ZARGS(rhs) ); }

            //! multiple prototype for operators
#define Y_MPZ_IMPL(OP,CALL) \
integer & operator OP##=(const integer  &rhs) { integer ans = CALL(*this,rhs); xch(ans); return *this; } \
integer & operator OP##=(const integer_t rhs) { integer ans = CALL(*this,rhs); xch(ans); return *this; } \
inline friend integer operator OP ( const integer  &lhs, const integer  &rhs ) { return CALL(lhs,rhs); } \
inline friend integer operator OP ( const integer  &lhs, const integer_t rhs ) { return CALL(lhs,rhs); } \
inline friend integer operator OP ( const integer_t lhs, const integer  &rhs ) { return CALL(lhs,rhs); } \
inline friend integer operator OP ( const integer  &lhs, const natural &rhs )  { return CALL(lhs,rhs); } \
inline friend integer operator OP ( const natural  &lhs, const integer &rhs )  { return CALL(lhs,rhs); }

            //! declaration and implementation of function for a given operator
#define Y_MPZ_WRAP(OP,CALL) Y_MPZ_DEFINE(integer,CALL) Y_MPZ_IMPL(OP,CALL)
            //__________________________________________________________________
            //
            // ADD
            //__________________________________________________________________
            Y_MPZ_WRAP(+,__add)

            //! unary plus
            inline integer operator+() { return *this; }

            //! increment
            inline integer __inc() const
            {
                const uint8_t b = 1;
                return __add(this->s,this->n.byte,this->n.bytes,__positive,&b,1);
            }

            //! pre increment operator
            integer & operator++()    { integer tmp = __inc(); xch(tmp); return *this; }

            //! post increment operator
            integer   operator++(int) { integer tmp = __inc(); xch(tmp); return tmp;   }

            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            Y_MPZ_WRAP(-,__sub)

            //! unary minus
            inline integer operator-() { return integer(sign_neg(s),n); }

            //! increment
            inline integer __dec() const
            {
                const uint8_t b = 1;
                return __add(this->s,this->n.byte,this->n.bytes,__negative,&b,1);
            }

            //! pre increment operator
            integer & operator--() { integer tmp = __dec(); xch(tmp); return *this; }

            //! post increment operator
            integer   operator--(int) { integer tmp = __dec(); xch(tmp); return tmp; }

            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPZ_WRAP(*,__mul)

            //__________________________________________________________________
            //
            // DIV
            //__________________________________________________________________
            Y_MPZ_WRAP(/,__div)

        private:
            static inline
            integer __add(const sign_type ls,
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

            static inline
            integer __sub(const sign_type ls,
                          const uint8_t  *l,
                          const size_t    nl,
                          const sign_type rs,
                          const uint8_t  *r,
                          const size_t    nr)
            {
                return __add(ls,l,nl,sign_neg(rs),r,nr);
            }

            static inline
            integer __mul(const sign_type ls,
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

            static
            integer __div(const sign_type ls,
                          const uint8_t  *l,
                          const size_t    nl,
                          const sign_type rs,
                          const uint8_t  *r,
                          const size_t    nr);
        };

    }

    typedef mpl::integer mpz; //!< alias for mp-signed
    namespace math
    {
        inline mpz fabs_of(const mpz &z) { return mpz(z.n); } //!< overloaded __fabs function
        inline mpz  __mod2(const mpz &z) { return z*z;      } //!< overloaded __mod2 function
    }

    //! extended numeric for mpz
    template <> struct xnumeric<mpz>
    {
        static mpz         abs_minimum()             { return mpz(); }                  //!< 0
        static inline bool is_zero(const mpz &z)     { return mpl::__zero     == z.s; } //!< z==0
        static inline bool is_positive(const mpz &z) { return mpl::__positive == z.s; } //!< z>0
    };

}


#endif

