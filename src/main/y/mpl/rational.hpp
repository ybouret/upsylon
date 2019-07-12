//! \file
#ifndef Y_RATIONAL_INCLUDED
#define Y_RATIONAL_INCLUDED 1

#include "y/mpl/integer.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! multiple precision rational
        class rational : public number_type
        {
        public:
            const integer num; //!< numerator
            const natural den; //!< denominator>0

            //! update to simplified form
            void check();

            //! default constructor, zero
            inline rational() : number_type(), num(),den(1) {}
            //! destructor
            inline virtual ~rational() throw() {}
            //! from integral type
            inline rational( const integer_t i ) :  object(), number_type(), num(i), den(1) {}
            //! from mpz
            inline rational( const integer & z ) :  object(), number_type(), num(z), den(1) {}
            //! from mpn
            inline rational( const natural & n ) :  object(), number_type(), num(n), den(1) {}
            //! from integral fraction
            inline rational( const integer_t n, const word_t  d) :  object(), number_type(), num(n), den(d) { check(); }
            //! from mpz/mpn
            inline rational( const integer &z, const natural &u) :  object(), number_type(), num(z), den(u) { check(); }

            //! output
            inline friend std::ostream & operator<<( std::ostream &os, const rational &q )
            {
                if(q.den.is_byte(1))
                {
                    os << q.num;
                }
                else
                {
                    os << '(' << q.num << '/' << q.den << ')';
                }
                return os;
            }

            //! conversion
            inline double to_real() const throw()
            {
                switch(num.s)
                {
                    case __negative: return -natural::ratio_of(num.n,den);
                    case __positive: return  natural::ratio_of(num.n,den);
                    case __zero:     return 0;
                }
#if defined(__ICC)
                fatal_error("intel corrupted code@mpl.rational.to_real");
                return 0;
#endif
#if defined(__GNUC__)
                fatal_error("gnu corrupted code@mpl.rational.to_real");
                return 0;
#endif
#if defined(_MSC_VER)
				fatal_error("MSC corrupted code@mpl.rational.to_real");
				return 0;
#endif
            }

            //! copy
            inline rational( const rational &q ) :  object(), number_type(), num(q.num), den(q.den) {}

            //! no throw exchange
            inline void xch( rational &q ) throw()
            {
                ( (integer &)num ).xch( (integer &)(q.num) );
                ( (natural &)den ).xch( (natural &)(q.den) );
            }

            //!assign
            inline rational & operator=( const rational &q )
            {
                rational tmp(q);
                xch(tmp);
                return *this;
            }

            //! assign from integral type
            inline rational & operator=( const integer_t i )
            {
                rational tmp(i); xch(tmp); return *this;
            }

            //! assign from integer type
            inline rational & operator=( const integer & z )
            {
                rational tmp(z); xch(tmp); return *this;
            }

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            //! basic comparions function
            static inline int compare( const rational &lhs, const rational &rhs)
            {
                const integer L = lhs.num * rhs.den;
                const integer R = rhs.num * lhs.den;
                return integer::compare(L,R);
            }

            //! changing RHS args
#define Y_MPQ_DEFINE_RHS(RET,BODY,CALL,TYPE)  inline RET BODY( const rational &lhs, const TYPE      rhs ) { const rational R(rhs); return CALL(lhs,R); }
            
            //! changing LHS args
#define Y_MPQ_DEFINE_LHS(RET,BODY,CALL,TYPE)  inline RET BODY( const TYPE      lhs, const rational &rhs ) { const rational L(lhs); return CALL(L,rhs); }
            
            //! defining change of args
#define Y_MPQ_DEFINE(RET,BODY,CALL,TYPE) Y_MPQ_DEFINE_RHS(RET,BODY,CALL,TYPE) Y_MPQ_DEFINE_LHS(RET,BODY,CALL,TYPE)

            //! overloaded operators
#define Y_MPQ_IMPL(RET,BODY,CALL)     \
Y_MPQ_DEFINE(RET,BODY,CALL,integer_t) \
Y_MPQ_DEFINE(RET,BODY,CALL,integer &) \
Y_MPQ_DEFINE(RET,BODY,CALL,natural &)
            
            //! overloaded function
#define Y_MPQ_OVERLOAD(RET,CALL) Y_MPQ_IMPL(RET,CALL,CALL)

            Y_MPQ_OVERLOAD(static int,compare)

            //! use it for comparison
#define Y_MPQ_CMP(OP) \
inline friend bool operator OP ( const rational &lhs, const rational &rhs ) { return compare(lhs,rhs) OP 0; }\
Y_MPQ_OVERLOAD(friend bool,operator OP)

            Y_MPQ_CMP(==)
            Y_MPQ_CMP(!=)
            Y_MPQ_CMP(<)
            Y_MPQ_CMP(<=)
            Y_MPQ_CMP(>)
            Y_MPQ_CMP(>=)

            //! use it for multiple operators API
#define Y_MPQ_WRAP(OP,CALL) \
inline rational & operator OP##=(const rational &q) { rational tmp = CALL(*this,q); xch(tmp); return *this; }\
inline rational & operator OP##=(const integer  &z) { const rational q(z); return (*this) OP##= q;          }\
inline rational & operator OP##=(const integer_t i) { const rational q(i); return (*this) OP##= q;          }\
inline friend rational operator OP (const rational &lhs, const rational &rhs ) { return CALL(lhs,rhs);      }\
Y_MPQ_IMPL(friend rational ,operator OP,CALL)

            //__________________________________________________________________
            //
            // ADD
            //__________________________________________________________________
            Y_MPQ_WRAP(+,__add)

            //! unary plus
            rational operator+() const
            {
                return *this;
            }

            //! increment
            inline rational __inc() const
            {
                const integer new_num = num + den;
                return rational(new_num,den);
            }

            //! pre increment operator
            inline rational & operator++()  { rational tmp = __inc(); xch(tmp); return *this; }

            //! post increment operator
            inline rational operator++(int) { rational tmp = __inc(); xch(tmp); return tmp; }

            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            Y_MPQ_WRAP(-,__sub)

            //! unary minus
            rational operator-() const
            {
                const integer new_num( sign_neg(num.s), num.n );
                return rational(new_num,den);
            }

            //! increment
            rational __dec() const
            {
                const integer new_num = num - den;
                return rational(new_num,den);
            }

            //! pre increment operator
            rational & operator--()  { rational tmp = __dec(); xch(tmp); return *this; }

            //! post increment operator
            rational operator--(int) { rational tmp = __dec(); xch(tmp); return tmp; }

            
            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPQ_WRAP(*,__mul)

            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPQ_WRAP(/,__div)

        private:
            void __simplify();
            static inline rational __add( const rational &lhs, const rational &rhs )
            {
                const natural dd = lhs.den * rhs.den;
                const integer ad = lhs.num * rhs.den;
                const integer cb = rhs.num * lhs.den;
                const integer nn = ad+cb;
                return rational(nn,dd);
            }

            static inline rational __sub( const rational &lhs, const rational &rhs )
            {
                const natural dd = lhs.den * rhs.den;
                const integer ad = lhs.num * rhs.den;
                const integer cb = rhs.num * lhs.den;
                const integer nn = ad-cb;
                return rational(nn,dd);
            }
            
            static inline rational __mul( const rational &lhs, const rational &rhs )
            {
                const integer new_num = lhs.num * rhs.num;
                const natural new_den = lhs.den * rhs.den;
                return rational(new_num,new_den);
            }
            
            static inline rational __div( const rational &lhs, const rational &rhs )
            {
                const integer i_num = lhs.num * rhs.den;
                const integer i_den = lhs.den * rhs.num;
                const integer o_num( sign_product(i_num.s,i_den.s), i_num.n);
                return rational(o_num,i_den.n);
            }
            
        };
    }

    typedef mpl::rational mpq; //!< alias
    namespace math
    {
        inline mpq fabs_of( const mpq &q ) { return mpq( fabs_of(q.num), q.den ); } //!< overload __fabs function
        inline mpq __mod2( const mpq &q )  { return q*q;                         } //!< overload __mod2 function
    }
    
    //! extended numeric for mpq
    template <> struct xnumeric<mpq> {
        static inline mpq  abs_minimum() { return mpq(); } //!< 0
        static inline bool is_zero(const mpq &q)     { return mpl::__zero     == q.num.s; } //!< q==0
        static inline bool is_positive(const mpq &q) { return mpl::__positive == q.num.s; } //!< q>0
    };
}

#endif

