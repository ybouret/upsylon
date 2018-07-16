//! \file
#ifndef Y_RATIONAL_INCLUDED
#define Y_RATIONAL_INCLUDED 1

#include "y/mpl/integer.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! multiple precision rational
        class rational : public object
        {
        public:
            const integer num;
            const natural den;

            //! update to simplified form
            void check();

            //! default constructor, zero
            inline rational() : num(),den(1) {}
            //! destructor
            inline virtual ~rational() throw() {}
            //! from integral type
            inline rational( const integer_t i ) : num(i), den(1) {}
            //! from mpz
            inline rational( const integer & z ) : num(z), den(1) {}
            //! from mpn
            inline rational( const natural & n ) : num(n), den(1) {}
            //! from integral fraction
            inline rational( const integer_t n, const word_t d) : num(n), den(d) { check(); }
            //! from mpz/mpn
            inline rational( const integer &z, const natural &u) : num(z), den(u) { check(); }

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
                critical_error("corrupted code@mpq.to_real");
                return 0;
#endif
            }

            //! copy
            inline rational( const rational &q ) : num(q.num), den(q.den) {}

            //! no throw exchange
            inline void xch( rational &q ) throw()
            {
                ( (integer &)num ).xch( (integer &)(q.num) );
                ( (integer &)den ).xch( (integer &)(q.den) );
            }

            //!assign
            inline rational & operator=( const rational &q )
            {
                rational tmp(q);
                xch(tmp);
                return *this;
            }

            inline rational & operator=( const integer_t i )
            {
                rational tmp(i); xch(tmp); return *this;
            }

            inline rational & operator=( const integer & z )
            {
                rational tmp(z); xch(tmp); return *this;
            }

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            static inline int compare( const rational &lhs, const rational &rhs)
            {
                const integer L = lhs.num * rhs.den;
                const integer R = rhs.num * lhs.den;
                return integer::compare(L,R);
            }

#define Y_MPQ_DEFINE_RHS(RET,BODY,CALL,TYPE)  inline RET BODY( const rational &lhs, const TYPE      rhs ) { const rational R(rhs); return CALL(lhs,R); }

#define Y_MPQ_DEFINE_LHS(RET,BODY,CALL,TYPE)  inline RET BODY( const TYPE      lhs, const rational &rhs ) { const rational L(lhs); return CALL(L,rhs); }

#define Y_MPQ_DEFINE(RET,BODY,CALL,TYPE) Y_MPQ_DEFINE_RHS(RET,BODY,CALL,TYPE) Y_MPQ_DEFINE_LHS(RET,BODY,CALL,TYPE)

#define Y_MPQ_IMPL(RET,BODY,CALL)     \
Y_MPQ_DEFINE(RET,BODY,CALL,integer_t) \
Y_MPQ_DEFINE(RET,BODY,CALL,integer &) \
Y_MPQ_DEFINE(RET,BODY,CALL,natural &)

#define Y_MPQ_OVERLOAD(RET,CALL) Y_MPQ_IMPL(RET,CALL,CALL)

            Y_MPQ_OVERLOAD(static int,compare)

#define Y_MPQ_CMP(OP) \
inline friend bool operator OP ( const rational &lhs, const rational &rhs ) { return compare(lhs,rhs) OP 0; }\
Y_MPQ_OVERLOAD(friend bool,operator OP)

            Y_MPQ_CMP(==)
            Y_MPQ_CMP(!=)
            Y_MPQ_CMP(<)
            Y_MPQ_CMP(<=)
            Y_MPQ_CMP(>)
            Y_MPQ_CMP(>=)

#define Y_MPQ_WRAP(OP,CALL) \
inline rational & operator OP##=(const rational &q) { rational tmp = CALL(*this,q); xch(tmp); return *this; }\
inline rational & operator OP##=(const integer  &z) { const rational q(z); return (*this) += q;             }\
inline rational & operator OP##=(const integer_t i) { const rational q(i); return (*this) += q;             }\
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
            rational __inc() const
            {
                const integer new_num = num + den;
                return rational(new_num,den);
            }

            //! pre increment operator
            rational & operator++()  { rational tmp = __inc(); xch(tmp); return *this; }

            //! post increment operator
            rational operator++(int) { rational tmp = __inc(); xch(tmp); return tmp; }

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
        };
    }

    typedef mpl::rational mpq;
}

#endif

