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

        //! operations on signs
        struct sign_ops
        {
            static sign_type product(const sign_type, const sign_type) throw(); //!< product of signs
            static sign_type of(const integer_t)   throw();                     //!< sign of integral type
            static sign_type of(const natural &)   throw();                     //!< sign of natural
            static word_t    i2w(const integer_t)  throw();                     //!< absolute value
            static sign_type neg( const sign_type) throw();                     //!< change sign
        };
        
        //! integer class
        class integer : public number_type
        {
        public:
            const sign_type s; //!< it sign
            const natural   n; //!< it natural value


            virtual ~integer() throw();                        //!< destructor
            integer();                                         //!< zero constructor
            integer(const integer   &z);                       //!< copy constructor
            integer(const integer_t &i);                       //!<  constructor from integral
            integer(const natural   &u);                       //!< constructor from natural
            integer(const sign_type _s, const natural &u );    //!< create from a natural and a given sign
            integer(const size_t nbit, randomized::bits &gen); //!< random integer
            void      update() throw();                        //!< status update
            integer_t lsi() const throw();                     //!< least significant int
            void      xch( integer &other ) throw();           //!< no throw exchange
            integer & operator=( const integer   &z);          //!< assign
            integer & operator=( const integer_t  i);          //!< assign
            integer & operator=( const natural   &u);          //!< assign

            //! output
            friend std::ostream & operator<<( std::ostream &, const integer &);

            //! get a signed decimal string
            string to_decimal() const;
            

            //! signed comparison
            static int compare_blocks(const sign_type ls,
                                      const uint8_t  *l,
                                      const size_t    nl,
                                      const sign_type rs,
                                      const uint8_t  *r,
                                      const size_t    nr) throw();

            //! binary presentation
            static const uint8_t *prepare( integer_t &i, sign_type &si, size_t &ni ) throw();

            //! raw data from integert_t
#define Y_MPZ_PREPARE() sign_type si = __zero; size_t ni=0; const uint8_t *bi = prepare(i,si,ni)
            //! present arguments from integer
#define Y_MPZ_ZARGS(V)  V.s,V.n.byte,V.n.bytes
            //! present arguments from integer_t
#define Y_MPZ_IARGS()   si,bi,ni
            //! present arguments from natural
#define Y_MPZ_UARGS()   sign_ops::of(u),u.byte,u.bytes
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
            integer operator+();

            //! increment
            integer __inc() const;

            //! pre increment operator
            integer & operator++();

            //! post increment operator
            integer   operator++(int);
            
            //__________________________________________________________________
            //
            // SUB
            //__________________________________________________________________
            Y_MPZ_WRAP(-,__sub)

            //! unary minus
            integer operator-() const;

            //! increment
            integer __dec() const;

            //! pre increment operator
            integer & operator--();

            //! post increment operator
            integer   operator--(int);

            //__________________________________________________________________
            //
            // MUL
            //__________________________________________________________________
            Y_MPZ_WRAP(*,__mul)
            static integer square_of( const integer &z ); //!< fast square

            //__________________________________________________________________
            //
            // DIV
            //__________________________________________________________________
            Y_MPZ_WRAP(/,__div)

            //__________________________________________________________________
            //
            // Serialize
            //__________________________________________________________________
            virtual const char *className() const throw();            //!< CLASS_NAME
            virtual size_t      serialize( ios::ostream &fp ) const;  //!< s and n
            static const char   CLASS_NAME[];                         //!< "mpz"
            static integer      read( ios::istream &, size_t *shift ); //!< retrieve a serialized integer

        private:
            static integer __add(const sign_type ls,
                                 const uint8_t  *l,
                                 const size_t    nl,
                                 const sign_type rs,
                                 const uint8_t  *r,
                                 const size_t    nr);

            static integer __sub(const sign_type ls,
                                 const uint8_t  *l,
                                 const size_t    nl,
                                 const sign_type rs,
                                 const uint8_t  *r,
                                 const size_t    nr);

            static integer __mul(const sign_type ls,
                                 const uint8_t  *l,
                                 const size_t    nl,
                                 const sign_type rs,
                                 const uint8_t  *r,
                                 const size_t    nr);

            static integer __div(const sign_type ls,
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
        mpz        sqrt_of(const mpz &z);                     //!< overloaded sqrt_of
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

