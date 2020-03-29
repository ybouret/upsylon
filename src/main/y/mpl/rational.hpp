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

            virtual ~rational() throw();                   //!< destructor
            rational();                                    //!< default constructor, zero
            rational(const integer_t);                     //!< from integral type
            rational( const integer & z );                 //!< from mpz
            rational( const natural & n );                 //!< from mpn
            rational( const integer_t n, const word_t  d); //!< from integral fraction
            rational( const integer &z, const natural &u); //!< from mpz/mpn
            rational( const rational &q );                 //!< copy
            rational & operator=( const rational &q );     //!< assign
            rational & operator=( const integer_t i );     //!< assign from integral type
            rational & operator=( const integer & z );     //!< assign from integer type
            rational & operator=( const natural & n );     //!< assign from natural
            void       check();                            //!< update to simplified form
            double     to_real() const throw();            //!< conversion
            void       xch( rational &q ) throw();         //!< no throw exchange

            //! output
            friend std::ostream & operator<<(std::ostream &, const rational &);

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________

            //! basic comparison function
            static int compare( const rational &lhs, const rational &rhs);

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
            rational   operator+() const; //!< unary plus
            rational    __inc() const;    //!< increment
            rational & operator++();      //!< pre increment operator
            rational   operator++(int);   //!< post increment operator

            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            Y_MPQ_WRAP(-,__sub)
            rational   operator-() const; //!< unary minus
            rational   __dec() const;     //!< increment
            rational & operator--();      //!< pre increment operator
            rational   operator--(int);   //!< post increment operator

            
            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPQ_WRAP(*,__mul)
            static rational square_of(const rational &q); //!< fast square

            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPQ_WRAP(/,__div)

            //__________________________________________________________________
            //
            // Serialize
            //__________________________________________________________________
            virtual const char *className() const throw();              //!< CLASS_NAME
            virtual size_t      serialize( ios::ostream &fp ) const;    //!< num and den
            static  const char  CLASS_NAME[];                           //!< "mpq"
            static  rational    read( ios::istream &fp, size_t *shift, const string &which); //!< read from serialized
            
        private:
            void __simplify();
            static  rational __add( const rational &lhs, const rational &rhs );
            static  rational __sub( const rational &lhs, const rational &rhs );
            static  rational __mul( const rational &lhs, const rational &rhs );
            static  rational __div( const rational &lhs, const rational &rhs );
            
        };
    }

    typedef mpl::rational mpq; //!< alias
    namespace math
    {
        inline mpq fabs_of( const mpq &q ) { return mpq( fabs_of(q.num), q.den ); } //!< overload __fabs function
        inline mpq __mod2( const mpq &q )  { return q*q;                         }  //!< overload __mod2 function
    }
    
    //! extended numeric for mpq
    template <> struct xnumeric<mpq> {
        static inline mpq  abs_minimum() { return mpq(); } //!< 0
        static inline bool is_zero(const mpq &q)     { return mpl::__zero     == q.num.s; } //!< q==0
        static inline bool is_positive(const mpq &q) { return mpl::__positive == q.num.s; } //!< q>0
    };
}

#endif

