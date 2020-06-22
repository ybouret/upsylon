//! \file
#ifndef Y_MP_NATURAL_INCLUDED
#define Y_MP_NATURAL_INCLUDED 1

#include "y/mpl/number.hpp"
#include "y/memory/buffer.hpp"
#include <iostream>

namespace upsylon {
    
    
    namespace randomized {
        class bits; //!< forward declaration
    }
    
    
    namespace mpl {
        
     
        
        //======================================================================
        //
        // multiple precision natural
        //
        //======================================================================
        
        //! check natural sanity
#define Y_MPN_CHECK(PTR)   do {                             \
assert(PTR); const natural &host = *PTR;                    \
assert( is_a_power_of_two(host.allocated) );                \
assert( host.bytes  <= host.allocated     );                \
assert( host.byte-1 == host.item          );                \
assert( (0 == host.bytes) || host.item[ host.bytes ]> 0 );  \
for(size_t ii=host.bytes;ii<host.allocated;++ii)            \
{ assert(0==host.byte[ii]); }                               \
} while(false)
        
        //! in place constructor
#define Y_MPN_CTOR(SZ,MX) object(), number_type(), memory::ro_buffer(),  bytes(SZ), allocated(MX), byte( __acquire(allocated) ), item(byte-1)
        
        class integer; //!< forward declaration
        
        //! big natural number
        class natural : public number_type, public memory::ro_buffer
        {
        public:
            static const char CLASS_NAME[]; //!< "mpn"
                                            //__________________________________________________________________
                                            //
                                            //
                                            // virtual interface
                                            //
                                            //__________________________________________________________________
            virtual const void  *ro()     const throw(); //!< buffer interface : ro
            virtual size_t       length() const throw(); //!< buffer interface : length
            
            //__________________________________________________________________
            //
            //
            // setup
            //
            //__________________________________________________________________
            natural();                                                    //!< a zero natural
            natural(size_t, const as_capacity_t &);                       //!< a zero with any memory
            virtual ~natural() throw();                                   //!< release memory
            natural(const natural &other);                                //!< copy
            natural(word_t w);                                            //!< setup from a word_type
            natural(const void  *buffer, size_t buflen);                  //!< copy from raw memory
            natural & operator=(const natural &);                         //!< assign
            natural & operator=(const word_t   );                         //!< assign from word_type
            natural(const size_t nbit, randomized::bits &gen);            //!< generate a random number
            void   xch( natural &other ) throw();                         //!< no throw exchange
            word_t lsw() const throw();                                   //!< Least Significant Word
            size_t bits() const throw();                                  //!< get number of bits
            void   clr() throw();                                         //!< clear memory
            static const uint8_t * prepare( word_t &, size_t & ) throw(); //!< prepare a scalar type
            
            natural(const char   *, const as_string_t &);                 //!< auto parsed text
            natural(const string &);                                      //!< auto parsed string
            natural & operator=( const char   * );                        //!< assign parsed text
            natural & operator=( const string & );                        //!< assign parsed text
            
            //__________________________________________________________________
            //
            //
            // I/O
            //
            //__________________________________________________________________
            //! formatted display
            std::ostream & display( std::ostream &) const;
            
            //! operator to std::ostream
            friend std::ostream & operator<<( std::ostream &os, const natural &n);
            
            //! convert to real value
            double to_real() const;
            
            //! get decimal string
            string to_decimal() const;
            
            //! expand the ratio
            static double ratio_of(const natural &num,const natural &den);
            
            static natural dec( const string &s ); //!< parse decimal string
            static natural dec( const char   *s ); //!< parse decimal text
            static natural hex( const string &s ); //!< parse hexadecimal string
            static natural hex( const char   *s ); //!< parse hexadecimal text
            static natural parse( const string &s ); //!< hexadecimal string if "0[xX]*", decimal otherwise
            static natural parse( const char   *s ); //!< parse text
            
            //__________________________________________________________________
            //
            //
            // comparisons
            //
            //__________________________________________________________________
            bool is_zero() const throw();                //!< fast checking against 0
            bool is_one()  const throw();                //!< fast checking against 0
            bool is_positive() const throw();            //!< fast checking greater than zero
            bool is_byte(const uint8_t x) const throw(); //!< fast checking against a byte
            void set_byte(const uint8_t x) throw();      //!< fast setting to a byte
            bool is_even() const throw();                //!< test if even
            bool is_odd() const throw();                 //!< test if odd
            
            //! comparison
            static
            int compare_blocks(const uint8_t *l,
                               const size_t   nl,
                               const uint8_t *r,
                               const size_t   nr) throw();
            
            //! inline preparation of a word
#define Y_MPN_PREPARE(W) size_t nw = 0; const uint8_t *pw = prepare(W,nw)
            
            //! multiple overloaded prototypes, no throw
#define Y_MPN_DEFINE_NOTHROW(RET,BODY,CALL) \
static inline RET BODY(const natural &lhs, const natural &rhs) throw() { return CALL(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_t         w  ) throw() { Y_MPN_PREPARE(w); return CALL(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_t         w,   const natural &rhs) throw() { Y_MPN_PREPARE(w); return CALL(pw,nw,rhs.byte,rhs.bytes); }
            
            Y_MPN_DEFINE_NOTHROW(int,compare,compare_blocks)
            
            //! comparison operator declarations
#define Y_MPN_CMP(OP) \
inline friend bool operator OP ( const natural  &lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const natural  &lhs, const word_t    rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const word_t    lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; }
            
            //! declare all operators
#define Y_MPN_CMP_DECL() \
Y_MPN_CMP(==)            \
Y_MPN_CMP(!=)            \
Y_MPN_CMP(<=)            \
Y_MPN_CMP(<)             \
Y_MPN_CMP(>=)            \
Y_MPN_CMP(>)
            
            Y_MPN_CMP_DECL()
            
            //! multiple overloaded prototypes
#define Y_MPN_DEFINE(RET,BODY) \
static inline RET BODY(const natural &lhs, const natural &rhs) { return BODY(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_t         w  ) { Y_MPN_PREPARE(w); return BODY(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_t         w,   const natural &rhs) { Y_MPN_PREPARE(w); return BODY(pw,nw,rhs.byte,rhs.bytes); }
            
            //! multiple prototype for operators
#define Y_MPN_IMPL(OP,CALL) \
natural & operator OP##=(const natural  &rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
natural & operator OP##=(const word_t    rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
inline friend natural operator OP ( const natural  &lhs, const natural  &rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const natural  &lhs, const word_t    rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const word_t    lhs, const natural  &rhs ) { return CALL(lhs,rhs); }
            
            //! declaration and implementation of function for a given operator
#define Y_MPN_WRAP(OP,CALL) Y_MPN_DEFINE(natural,CALL) Y_MPN_IMPL(OP,CALL)
            
            //__________________________________________________________________
            //
            //
            // ADD
            //
            //__________________________________________________________________
            Y_MPN_WRAP(+,__add)
            natural   operator+() const; //!< unary plus
            natural   __inc() const;     //!< increase by 1
            natural & operator++();      //!< prefix increase operator
            natural   operator++(int);   //!< postfix increase operator
            
            //__________________________________________________________________
            //
            //
            // SUB, no unary '-'
            //
            //__________________________________________________________________
            Y_MPN_WRAP(-,__sub)
            natural   __dec() const;   //!< decrease by 1
            natural & operator--();    //!< prefix decrease operator
            natural   operator--(int); //!< postfix decrease operator
            
            //__________________________________________________________________
            //
            //
            // MUL
            //
            //__________________________________________________________________
            Y_MPN_WRAP(*,__mul)
            static natural square_of(const natural &n ); //!< fast square
            static natural short_mul(const natural &lhs, const uint8_t x); //!< short multiplication
            
            //__________________________________________________________________
            //
            //
            // Bits shifting
            //
            //__________________________________________________________________
            uint8_t        operator[](size_t indx)    const throw();         //!< ready any byte
            uint8_t        get_bit(const size_t ibit) const throw();         //!< ready valid bit to 0 or 1
            bool           has_bit(const size_t ibit) const throw();         //!< test a valid bit
            natural        shl(const size_t shift) const;                    //!< left shift
            natural &      shl();                                            //!< in place left shift
            natural &      operator<<=(const size_t shift);                  //!< in place left shift operator
            friend natural operator<<(const natural &n,const size_t shift);  //!< left shift operator
            natural        shr(const size_t shift) const;                    //!< right shift
            natural &      shr();                                            //!< in place right shift
            natural &      operator>>=(const size_t shift);                  //!< in place right shift operator
            friend natural operator>>(const natural &n, const size_t shift); //!< left shift operator
            static natural exp2( const size_t j );                           //!< 2^j
            
            //__________________________________________________________________
            //
            //
            // DIV
            //
            //__________________________________________________________________
            Y_MPN_WRAP(/,__div)
            //! num = q*den+r
            static  void split( natural &q, natural &r, const natural &num, const natural &den );
            
            //__________________________________________________________________
            //
            //
            // MOD
            //
            //__________________________________________________________________
            Y_MPN_WRAP(%,__mod)
            
            //__________________________________________________________________
            //
            //
            // DIVISIBILITY
            //
            //__________________________________________________________________
            bool is_divisible_by( const natural &rhs ) const; //!< test divisibility
            bool is_divisible_by(word_t w) const;             //!< test divisibility
            bool is_divisible_by_byte(const uint8_t b) const; //!< test divisibility
            
            //__________________________________________________________________
            //
            //
            // boolean ops
            //
            //__________________________________________________________________
            //! byte-wise operator
            typedef uint8_t (*booleanOp)(const uint8_t lhs, const uint8_t rhs);
            Y_MPN_WRAP(&,__and)
            Y_MPN_WRAP(|,__or)
            Y_MPN_WRAP(^,__xor)
            
            //__________________________________________________________________
            //
            //
            // Arithmetic
            //
            //__________________________________________________________________
            static natural mod_inv(const natural &b, const natural &n);                     //!< modular inverse
            static natural mod_exp(const natural &b, const natural &e, const natural &n);   //!< modular exponentiation (b^e)[n]
            static bool    are_coprimes(const natural &, const natural &);                  //!< test co-primailty
            static natural factorial(const natural &n);                                     //!< factorial computation, recursive algorithm
            static natural factorial(const word_t   n);                                     //!< factorial computation, wrapper
            static natural square_root_of(const natural &n);                                //!< integer square root of n
            
            //__________________________________________________________________
            //
            //
            // io
            //
            //__________________________________________________________________
            virtual const char *className() const throw();              //!< "mpn"
            virtual size_t      serialize( ios::ostream &fp ) const;    //!< write binary
            static natural      read(ios::istream &fp, size_t &shift, const char *which);  //!< reload
            
            //! implemented for uint8_t, uint16_t, uint32_t, uint64_t
            template <typename T>
            bool to(T &target) const throw();
            
            //! convert to integral type
            template <typename T>
            inline bool as(T &target) const throw()
            {
                typedef typename unsigned_int<sizeof(T)>::type utype;
                return to<utype>( (utype&)target );
            }
            
            //! wrapper
            template <typename T>
            inline T cast_to(const char *when = 0) const
            {
                T ans(0);
                if(!as<T>(ans))
                {
                    throw_cast_overflow(when);
                }
                return ans;
            }
            
            
        private:
            size_t   bytes;     //!< active bytes
            size_t   allocated; //!< allocated bytes
            uint8_t *byte;      //!< byte[0..allocated-1]
            uint8_t *item;      //!< item[1..allocated]
            
            void throw_cast_overflow(const char *when) const;
            void update()  throw(); //!< from bytes
            void upgrade() throw(); //!< set bytes to allocated and update
            
            static  uint8_t * __acquire(size_t &n);
            
            natural __shl(const size_t shift) const;
            
            static natural __add(const uint8_t *l,
                                 const size_t   nl,
                                 const uint8_t *r,
                                 const size_t   nr);
            
            static natural __sub(const uint8_t *l,
                                 const size_t   nl,
                                 const uint8_t *r,
                                 const size_t   nr);
            
            static natural __mul(const uint8_t *l, const size_t nl,
                                 const uint8_t *r, const size_t nr);
            
            static natural __div(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);
            
            static natural __mod(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);
            
            //! true if divisible
            static bool    __dvs(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);
            
            static natural __bool(const uint8_t *l, const size_t nl,
                                  const uint8_t *r, const size_t nr,
                                  booleanOp  proc);
            //! low level boolean functions implementation
#define Y_MPN_BOOL(CALL,OP) \
static inline uint8_t __##CALL##__(const uint8_t a, const uint8_t b ) throw() { return (a OP b); }\
static inline natural __##CALL(const uint8_t *l, const size_t nl, const uint8_t *r, const size_t nr) { return __bool(l,nl,r,nr,__##CALL##__); }
            Y_MPN_BOOL(and,&)
            Y_MPN_BOOL(or,|)
            Y_MPN_BOOL(xor,^)
            
            friend class integer;
            
            static void    __smul(uint8_t       *b,
                                  size_t        &m,
                                  const uint8_t *a,
                                  const size_t   n,
                                  const uint8_t  x) throw();
        };
        
    }
}


#include "y/type/xnumeric.hpp"

namespace upsylon {
    
    typedef mpl::natural   mpn; //!< alias
    namespace mkl
    {
        inline mpn fabs_of(const mpn &u) { return u;   } //!< overloaded __fabs function
        inline mpn  __mod2(const mpn &u) { return u*u; } //!< overloaded __mod2 function
        inline mpn sqrt_of(const mpn &u) { return mpn::square_root_of(u); } //!< overloaded sqrt_of function
    }
    
    //! extended numeric for mpn
    template <> struct xnumeric<mpn>
    {
        static inline mpn  abs_minimum() { return mpn(); } //!< 0
        static inline bool is_zero(const mpn &x)     { return x.is_zero();     } //!< x==0
        static inline bool is_positive(const mpn &x) { return x.is_positive(); } //!< x>0
    };
    
}


#endif

