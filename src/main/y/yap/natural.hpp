
//! \file
#ifndef Y_AP_NATURAL_INCLUDED
#define Y_AP_NATURAL_INCLUDED 1

#include "y/yap/number.hpp"
#include "y/memory/tight/twain-allocator.hpp"
#include "y/strfwd.hpp"

namespace upsylon {

    namespace randomized {

        class bits;
    }

    namespace yap {
        //______________________________________________________________________
        //
        // declarations
        //______________________________________________________________________
        //#define Y_YAP_FORCE16
        class library;                                            //!< forward declaration
        typedef memory::tight::twain_allocator memory_allocator; //!< alias


        //______________________________________________________________________
        //
        //
        //! natural drop-in unsigned replacement
        //
        //______________________________________________________________________
        class natural : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLASS_NAME[]; //!< "apn"

            //------------------------------------------------------------------
            // core type, to perform internal computations
            //------------------------------------------------------------------
            typedef typename unsigned_int<sizeof(void*)>::type core_type;                     //!< system native unsigned
            static const size_t                                core_size = sizeof(core_type); //!< system native size
            static const size_t                                core_bits = core_size << 3;    //!< system native  bits

            //------------------------------------------------------------------
            // word type, to handle memory
            //------------------------------------------------------------------
#if defined(Y_YAP_FORCE16)
            typedef uint16_t                                    word_type; //!< memory of word_type
#else
            typedef typename unsigned_int<(core_size>>1)>::type word_type; //!< memory of word_type
#endif
            static const size_t                                word_size  = sizeof(word_type);            //!< word size
            static const size_t                                word_bits  = word_size << 3;               //!< word bits
            static const size_t                                word_exp2  = ilog2<word_size>::value;      //!< word_size = 1 << word_exp2
            static const size_t                                word_mask  = word_size-1;                  //!< word_size - 1 = least significant bits
            static const core_type                             word_max   = limit_of<word_type>::maximum; //!< maximum  for core_type
            static const core_type                             word_radix = core_type(1) << word_bits;    //!< radix    for core_type
            static const size_t                                words_per_utype = sizeof(utype)/word_size; //!< number of words per utype

            //__________________________________________________________________
            //
            // C++ and constructors
            //__________________________________________________________________
            virtual ~natural() throw();                    //!< cleanup
            natural();                                     //!< zero
            natural(const size_t , const as_capacity_t &); //!< zero with memory for BYTES
            natural(const natural &);                      //!< copy
            natural(utype);                                //!< build from utype
            natural & operator=(const natural &);          //!< assign
            natural & operator=(const utype);              //!< assign from utype
            natural(randomized::bits &, const size_t nbit);//!< bits() = random bits

            //__________________________________________________________________
            //
            // management methods
            //__________________________________________________________________
            void   xch(natural &) throw(); //!< no-throw exchange
            utype  lsw()    const throw(); //!< least significant words
            size_t bits()   const throw(); //!< number of bits
            size_t size()   const throw(); //!< bytes
            size_t wc()     const throw(); //!< words
            void   ldz()    throw();       //!< clear all
            
            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char *className()       const throw(); //!< CLASS_NAME
            virtual size_t      serialize(ios::ostream &) const; //!< portable output
            static  natural     read(ios::istream &, size_t &, const char *); //!< relaod

            //__________________________________________________________________
            //
            // macros for overloading
            //__________________________________________________________________

            //! inline utype -> word_type *
#define Y_APN_U2W(args)         \
volatile utype   u  = (args);   \
size_t           nw = 0;        \
const word_type *pw = u2w(u,nw)

            //! natural,natural
#define Y_APN_ARGS_N_N (const natural &lhs, const natural &rhs)
            //! utype,natural
#define Y_APN_ARGS_U_N (const utype    lhs, const natural &rhs)
            //! natural,utype
#define Y_APN_ARGS_N_U (const natural &lhs, const utype    rhs)  

            //! overload a CALL from words operating function
#define Y_APN_OVERLOAD_NO_THROW(RETURN,CALL) \
inline static RETURN CALL Y_APN_ARGS_N_N throw() { return CALL(lhs.word,lhs.words,rhs.word,rhs.words);    }\
inline static RETURN CALL Y_APN_ARGS_U_N throw() { Y_APN_U2W(lhs); return CALL(pw,nw,rhs.word,rhs.words); }\
inline static RETURN CALL Y_APN_ARGS_N_U throw() { Y_APN_U2W(rhs); return CALL(lhs.word,lhs.words,pw,nw); }

            //! implement one method
#define Y_APN_IMPL_METHOD_(PREFIX,ARGS,SUFFIX) PREFIX ARGS SUFFIX

            //! implement the method with different arguments
#define Y_APN_IMPL_METHODS(PREFIX,CODE)\
Y_APN_IMPL_METHOD_(PREFIX,Y_APN_ARGS_N_N,CODE)\
Y_APN_IMPL_METHOD_(PREFIX,Y_APN_ARGS_U_N,CODE)\
Y_APN_IMPL_METHOD_(PREFIX,Y_APN_ARGS_N_U,CODE)

            //__________________________________________________________________
            //
            // total comparisons
            //__________________________________________________________________
            //! call operator and function
#define Y_APN_IMPL_COMPARE_TOTAL(OP,CALL) Y_APN_IMPL_METHODS(inline friend bool operator OP,throw() { return CALL(lhs,rhs);  })
            Y_APN_IMPL_COMPARE_TOTAL(==,eq)
            Y_APN_IMPL_COMPARE_TOTAL(!=,neq)
            Y_APN_OVERLOAD_NO_THROW(int,cmp)

            //__________________________________________________________________
            //
            // partial comparison
            //__________________________________________________________________

            //! build partial comparators
#define Y_APN_WRAP_CMP(OP) Y_APN_IMPL_METHODS(inline friend bool operator OP, throw() { return cmp(lhs,rhs) OP 0; })

            //! build all partial comparators
#define Y_APN_IMPL_COMPARE_PARTIAL() \
Y_APN_WRAP_CMP(< )               \
Y_APN_WRAP_CMP(<=)               \
Y_APN_WRAP_CMP(> )               \
Y_APN_WRAP_CMP(>=)

            Y_APN_IMPL_COMPARE_PARTIAL()

            //! for different sorting algorithms
            static inline int compare(const natural &lhs, const natural &rhs) throw() { return cmp(lhs,rhs); }

            //__________________________________________________________________
            //
            // for fast branching
            //__________________________________________________________________
            Y_APN_OVERLOAD_NO_THROW(sign_type,scmp)

            //__________________________________________________________________
            //
            // additions
            //__________________________________________________________________

            
            //! complete API
#define Y_APN_OVERLOAD_API(CALL) \
inline static natural CALL(const natural &lhs, const natural &rhs) { return CALL(lhs.word,lhs.words,rhs.word,rhs.words);    }\
inline static natural CALL(const utype    lhs, const natural &rhs) { Y_APN_U2W(lhs); return CALL(pw,nw,rhs.word,rhs.words); }\
inline static natural CALL(const natural &lhs, const utype    rhs) { Y_APN_U2W(rhs); return CALL(lhs.word,lhs.words,pw,nw); }

            //! complete binary operators
#define Y_APN_OPERATOR_API(OP,CALL) \
inline natural & operator OP##=( const natural &rhs) { natural tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline natural & operator OP##=( const utype    rhs) { natural tmp = CALL(*this,rhs); xch(tmp); return *this; }\
Y_APN_IMPL_METHODS(inline friend natural operator OP,{ return CALL(lhs,rhs); })


            //! higher level wrapper, requiring natural args
#define Y_APN_OVERLOAD_HL(RETURN,CALL) \
static inline RETURN CALL(const natural &lhs, const utype    rhs) { const natural tmp(rhs); return CALL(lhs,tmp); }\
static inline RETURN CALL(const utype    lhs, const natural &rhs) { const natural tmp(lhs); return CALL(tmp,rhs); }

            //! implement methods and operator from call
#define Y_APN_STANDARD_API(OP,CALL) \
Y_APN_OVERLOAD_API(CALL)            \
Y_APN_OPERATOR_API(OP,CALL)

            //! implement methods and operator from call
#define Y_APN_ADVANCED_API(OP,CALL) \
Y_APN_OVERLOAD_HL(natural,CALL)     \
Y_APN_OPERATOR_API(OP,CALL)

            Y_APN_STANDARD_API(+,add)
            natural   operator+() const; //!< unary +
            natural & operator++();      //!< prefix++ operator
            natural   operator++(int);   //!< postfix++ operator

            //__________________________________________________________________
            //
            // subtraction, no unitary -
            //__________________________________________________________________
            Y_APN_STANDARD_API(-,sub)
            natural & operator--();      //!< prefix-- operator
            natural   operator--(int);   //!< postfix-- operator


            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            Y_APN_STANDARD_API(*,mul)

            static natural square_of(const natural &x); //!< x*x

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________


            //! divide algorithm
            static natural divide(const natural &num, const natural &den);
            Y_APN_ADVANCED_API(/,divide)
            static natural sqrt_of(const natural &x); //!< sqrt(x)^2 <= x
            static natural sqrt_of(const utype    u); //!< sqrt(u)^2 <= u

            //__________________________________________________________________
            //
            // modulo
            //__________________________________________________________________

            //! modulo algorithm
            static natural modulo(const natural &num, const natural &den);
            Y_APN_ADVANCED_API(%,modulo)

            static bool divisible(const natural &num, const natural &den); //!< test divisibility
            bool        is_divisible_by(const natural &) const; //!< test divisibility
            bool        is_divisible_by(const utype    ) const; //!< test divisibility


            //__________________________________________________________________
            //
            // split
            //__________________________________________________________________
            //! num = q*den+r
            static void split(natural &q, natural &r, const natural &num, const natural &den);


            //__________________________________________________________________
            //
            // bit shifting
            //__________________________________________________________________
            static natural exp2( const size_t nbits );                 //!< 1 << nbits
            bool           bit(const size_t ibit) const throw();       //!< get the bit status
            natural       &shr(const size_t s) throw();                //!< in-place right shift, no-throw
            natural       &shl(const size_t s);                        //!< left shift, would throw
            friend natural operator >> (const natural &,const size_t); //!< right shift
            friend natural operator << (const natural &,const size_t); //!< left shift
            natural & operator<<=(const size_t s);                     //!< left shift
            natural & operator>>=(const size_t s) throw();             //!< right shift
            
            //__________________________________________________________________
            //
            // bitwise logical
            //__________________________________________________________________
            Y_APN_OPERATOR_API(|,_or)
            Y_APN_OPERATOR_API(&,_and)
            Y_APN_OPERATOR_API(^,_xor)

            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static memory_allocator &instance();                                       //!< internal dedicated memory
            static memory_allocator &location() throw();                               //!< internal dedicated memory
            static size_t            words_for(const size_t) throw();                  //!< words to hold some bytes
            static bool              check(const natural &, const char *) throw();     //!< debug checking
            static const word_type  *u2w(volatile utype &, volatile size_t &) throw(); //!< utype -> word_type *

            //! output
            friend std::ostream     &operator<<(std::ostream &, const natural &);
            
            //__________________________________________________________________
            //
            // parsing
            //__________________________________________________________________
            string         to_dec() const;                    //!< to decimal string
            string         to_hex() const;                    //!< to hexadecimal string
            static natural dec(const char *, const size_t);   //!< parse decimal
            static natural dec(const char   *);               //!< parse decimal
            static natural dec(const string &);               //!< parse decimal
            static natural hex(const char * ,const size_t);   //!< parse hexadecimal
            static natural hex(const char   *);               //!< parse hexadecimal
            static natural hex(const string &);               //!< parse hexadecimal
            static natural parse(const char *, const size_t); //!< parse depending on "0x"
            static natural parse(const char *);               //!< parse wrapper
            static natural parse(const string &);             //!< parse wrapper

            //__________________________________________________________________
            //
            // floating point
            //__________________________________________________________________
            double         to_double() const;                                //!< convert to a double value
            static double  ratio_of(const natural &num, const natural &den); //!< convert ratio to double
            
            //__________________________________________________________________
            //
            // arithmethic
            //__________________________________________________________________
            static natural factorial(size_t n);                      //!< n!
            static natural comb(const size_t n, const size_t k);     //!< n!/k!/(n-k)!
            static natural gcd(const natural &x, const natural &y);  //!< gcd(x>0,y>0)
            Y_APN_OVERLOAD_HL(natural,gcd)
            static natural lcm(const natural &x, const natural &y);  //!< lcm(x>0,y>0) = x*y/gcd(x,y)
            Y_APN_OVERLOAD_HL(natural,lcm)
            static void    simplify(natural &num, natural &den);                            //!< simplify fraction
            static natural mod_inv(const natural &b, const natural &n);                     //!< modular inverse        (b^(-1))[n]
            static natural mod_exp(const natural &b, const natural &e, const natural &n);   //!< modular exponentiation (b^e)[n]
            static bool    coprime(const natural &lhs,const natural &rhs);                  //!< 1==gcd(lhs,rhs)
            Y_APN_OVERLOAD_HL(bool,coprime)

            //__________________________________________________________________
            //
            // conversion
            //__________________________________________________________________
            //! convert to an integral type
            template <typename T> inline
            bool to( T &target ) const throw()
            {
                static const T     tmax = limit_of<T>::maximum; assert(tmax>0);
                static const utype umax(tmax);
                if( *this > umax )
                {
                    return false;
                }
                else
                {
                    target = T(lsw());
                    return true;
                }
            }

            //! wrapper
            template <typename T>
            inline T cast_to(const char *when = 0) const
            {
                T ans(0);
                if(!to<T>(ans))
                {
                    cast_overflow(when);
                }
                return ans;
            }

            //__________________________________________________________________
            //
            // specific
            //__________________________________________________________________
            void make(const word_type w) throw();     //!< set to one word (safe)
            bool is(const word_type w) const throw(); //!< check is one word with given value
            bool is_even() const throw();             //!< check if even
            bool is_odd()  const throw();             //!< check if odd

            //__________________________________________________________________
            //
            // logarithm
            //__________________________________________________________________
            static natural log_of(const natural &);

        private:
            size_t     bytes; //!< effective number of bytes
            size_t     words; //!< effective number of words = words_for(bytes)
            size_t     count; //!< memory words count
            size_t     width; //!< memory bytes count
            size_t     shift; //!< width = 1 << shift
            word_type *word;  //!< memory area
            
            static word_type *acquire(size_t &count, size_t &width, size_t &shift);
            static void       release(word_type *, size_t &count, size_t &width, size_t &shift) throw();
            static uint8_t   &get_byte(const word_type *w, const size_t i);
            static bool       get_bit(const word_type *w, const size_t ibit) throw();
            static void       set_bit(const word_type *w, const size_t ibit) throw();
            static void       clr_bit(const word_type *w, const size_t ibit) throw();
            static void       cast_overflow(const char *); //!< throw on overflow

            uint8_t &get(const size_t i) const throw(); //!< LE byte in [0..width-1]


            void update()  throw(); //!< check bytes from current position
            void upgrade() throw(); //!< set bytes to width and update

            //! test words equality
            static bool eq(const word_type *lhs, const size_t lnw,
                           const word_type *rhs, const size_t rnw) throw();
            Y_APN_OVERLOAD_NO_THROW(bool,eq)

            //! test words difference
            static bool neq(const word_type *lhs, const size_t lnw,
                            const word_type *rhs, const size_t rnw) throw();
            Y_APN_OVERLOAD_NO_THROW(bool,neq)

            //! comparison
            static int      cmp(const word_type *lhs, const size_t lnw,
                                const word_type *rhs, const size_t rnw) throw();

            static sign_type scmp(const word_type *lhs, const size_t lnw,
                                  const word_type *rhs, const size_t rnw) throw();

            //! addition
            static natural add(const word_type *lhs, const size_t lnw,
                               const word_type *rhs, const size_t rnw);

            //! addition
            static natural sub(const word_type *lhs, const size_t lnw,
                               const word_type *rhs, const size_t rnw);

            //! multiplicaion
            static natural mul(const word_type *lhs, const size_t lnw,
                               const word_type *rhs, const size_t rnw);

            typedef word_type (*boolean_op)(const word_type,const word_type);
            
            //! implementing logical operators
#define Y_APN_LOGICAL(FCN,OP) \
static inline word_type _##FCN(const word_type lhs, const word_type rhs) throw() { return (lhs OP rhs); }\
static inline natural      FCN(const word_type *lhs, const size_t lnw, const word_type *rhs, const size_t rnw) \
{ return logical(_##FCN,lhs,lnw,rhs,rnw); }
            
            Y_APN_LOGICAL(_or,|)  Y_APN_OVERLOAD_API(_or)
            Y_APN_LOGICAL(_and,&) Y_APN_OVERLOAD_API(_and)
            Y_APN_LOGICAL(_xor,^) Y_APN_OVERLOAD_API(_xor)

            //! boolean operators
            static natural logical(boolean_op op,
                                   const word_type *lhs, const size_t lnw,
                                   const word_type *rhs, const size_t rnw);
            
            friend class library;

        };

    }

    typedef yap::natural apn; //!< nickname

    namespace mkl {
        inline apn fabs_of(const apn &x) { return x;                 } //!< for use in mkl
        inline apn sqrt_of(const apn &x) { return apn::sqrt_of(x);   } //!< for use in mkl
        inline apn mod2_of(const apn &x) { return apn::square_of(x); } //!< for use un mkl
    }

}


#endif
